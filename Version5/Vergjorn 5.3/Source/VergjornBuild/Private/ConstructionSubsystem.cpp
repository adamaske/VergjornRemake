#include "ConstructionSubsystem.h"
#include "BuildingDefinition.h"
#include "GameplayMessageSubsystem.h"
#include "VergjornTags.h"

DEFINE_LOG_CATEGORY(LogConstruction);

void UConstructionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Collection.InitializeDependency<UEconomySubsystem>();
	Collection.InitializeDependency<UJobSubsystem>();
}

FIntPoint UConstructionSubsystem::WorldToGrid(const FVector& WorldPos) const
{
	return FIntPoint(
		FMath::FloorToInt(WorldPos.X / CellSize),
		FMath::FloorToInt(WorldPos.Y / CellSize)
	);
}

FVector UConstructionSubsystem::GridToWorld(const FIntPoint& GridPos, bool bCenterOfCell) const
{
	const float Offset = bCenterOfCell ? CellSize * 0.5f : 0.f;
	return FVector(GridPos.X * CellSize + Offset, GridPos.Y * CellSize + Offset, 0.f);
}

bool UConstructionSubsystem::CanPlace(const FIntPoint& Origin, const FIntPoint& Footprint) const
{
	for (int32 X = 0; X < Footprint.X; ++X)
	{
		for (int32 Y = 0; Y < Footprint.Y; ++Y)
		{
			const FGridCell* Cell = Grid.Find(FIntPoint(Origin.X + X, Origin.Y + Y));
			if (Cell && Cell->IsOccupied()) return false;
		}
	}
	return true;
}

int32 UConstructionSubsystem::PlaceBuilding(UBuildingDefinition* Definition, const FIntPoint& GridOrigin)
{
	if (!Definition)
	{
		UE_LOG(LogConstruction, Warning, TEXT("PlaceBuilding: null definition"));
		return INDEX_NONE;
	}
	if (!CanPlace(GridOrigin, Definition->FootprintCells))
	{
		UE_LOG(LogConstruction, Warning, TEXT("PlaceBuilding: occupied at %d,%d"), GridOrigin.X, GridOrigin.Y);
		return INDEX_NONE;
	}

	const int32 BuildingId = NextBuildingId++;
	OccupyCells(GridOrigin, Definition->FootprintCells, BuildingId);

	AVergBuilding* BuildingActor = nullptr;
	if (UClass* BuildingClass = Definition->BuildingClass.LoadSynchronous())
	{
		const FVector WorldLoc = GridToWorld(GridOrigin, false);
		BuildingActor = GetWorld()->SpawnActor<AVergBuilding>(BuildingClass, WorldLoc, FRotator::ZeroRotator);
		if (BuildingActor)
		{
			BuildingActor->SetDefinition(Definition);
			BuildingActor->SetBuildingId(BuildingId);
			BuildingActor->GridOrigin = GridOrigin;
			BuildingActor->OnStateChanged(EBuildingState::Planned);
		}
	}

	FConstructionSite Site;
	Site.BuildingId    = BuildingId;
	Site.GridOrigin    = GridOrigin;
	Site.Definition    = Definition;
	Site.BuildingActor = BuildingActor;
	Site.Phase         = EConstructionPhase::Planned;
	Sites.Add(BuildingId, Site);

	AdvanceToMaterialsPhase(Sites[BuildingId]);

	UE_LOG(LogConstruction, Log, TEXT("Placed: %s (Id=%d) @ %d,%d"),
		*Definition->DisplayName.ToString(), BuildingId, GridOrigin.X, GridOrigin.Y);
	return BuildingId;
}

void UConstructionSubsystem::AdvanceToMaterialsPhase(FConstructionSite& Site)
{
	UEconomySubsystem* Economy = GetWorld()->GetSubsystem<UEconomySubsystem>();
	if (!Economy) return;

	bool bAllReserved = true;
	for (auto& [Resource, Amount] : Site.Definition->ConstructionCost)
	{
		FReservationHandle Handle;
		if (Economy->TryReserve(Resource, Amount, Handle))
		{
			Site.MaterialReservations.Add(Resource, Handle);
		}
		else
		{
			bAllReserved = false;
			for (auto& [R, H] : Site.MaterialReservations)
			{
				Economy->ReleaseReservation(const_cast<FReservationHandle&>(H));
			}
			Site.MaterialReservations.Empty();
			break;
		}
	}

	if (bAllReserved)
	{
		Site.Phase = EConstructionPhase::MaterialsReserved;
		AdvanceToBuildPhase(Site);
	}
	else
	{
		UE_LOG(LogConstruction, Log, TEXT("Building %d waiting for materials"), Site.BuildingId);
	}
}

void UConstructionSubsystem::AdvanceToBuildPhase(FConstructionSite& Site)
{
	Site.Phase = EConstructionPhase::Hauling;
	if (Site.BuildingActor.IsValid())
	{
		Site.BuildingActor->OnStateChanged(EBuildingState::Hauling);
	}
	PostHaulJobs(Site);
}

void UConstructionSubsystem::PostHaulJobs(FConstructionSite& Site)
{
	UJobSubsystem* Jobs = GetWorld()->GetSubsystem<UJobSubsystem>();
	if (!Jobs) return;

	const FVector Loc = GridToWorld(Site.GridOrigin);
	for (auto& [Resource, Reservation] : Site.MaterialReservations)
	{
		FJobRecord Job;
		Job.JobType              = TAG_Job_Haul;
		Job.RequiredOccupation   = FGameplayTag();  // any villager can haul
		Job.Location             = Loc;
		Job.Priority             = 10;
		Job.WorkUnitsRequired    = 5.f;
		Job.LinkedReservation    = Reservation;
		Site.ActiveJobs.Add(Jobs->PostJob(Job));
	}
}

void UConstructionSubsystem::PostBuildJobs(FConstructionSite& Site)
{
	UJobSubsystem* Jobs = GetWorld()->GetSubsystem<UJobSubsystem>();
	if (!Jobs) return;

	FJobRecord Job;
	Job.JobType            = TAG_Job_Build;
	Job.RequiredOccupation = TAG_Occupation_Builder;
	Job.Location           = GridToWorld(Site.GridOrigin);
	Job.Priority           = 20;
	Job.WorkUnitsRequired  = Site.Definition->BuildWorkUnits;
	Job.TargetActor        = Site.BuildingActor.Get();
	Site.ActiveJobs.Add(Jobs->PostJob(Job));
}

void UConstructionSubsystem::OnHaulJobCompleted(int32 BuildingId, FGameplayTag Resource, int32 Amount)
{
	FConstructionSite* Site = Sites.Find(BuildingId);
	if (!Site || Site->Phase != EConstructionPhase::Hauling) return;

	UEconomySubsystem* Economy = GetWorld()->GetSubsystem<UEconomySubsystem>();
	if (Economy)
	{
		if (FReservationHandle* Res = Site->MaterialReservations.Find(Resource))
		{
			Economy->CommitReservation(*Res);
			Site->MaterialReservations.Remove(Resource);
		}
	}

	if (Site->MaterialReservations.IsEmpty())
	{
		Site->Phase = EConstructionPhase::Building;
		if (Site->BuildingActor.IsValid())
		{
			Site->BuildingActor->OnStateChanged(EBuildingState::Building);
		}
		PostBuildJobs(*Site);
	}
}

void UConstructionSubsystem::OnBuildJobCompleted(int32 BuildingId, float WorkUnits)
{
	FConstructionSite* Site = Sites.Find(BuildingId);
	if (!Site || Site->Phase != EConstructionPhase::Building) return;

	Site->WorkUnitsDone += WorkUnits;
	if (Site->BuildingActor.IsValid() && Site->Definition)
	{
		Site->BuildingActor->ConstructionProgress =
			FMath::Min(1.f, Site->WorkUnitsDone / Site->Definition->BuildWorkUnits);
	}

	if (Site->WorkUnitsDone >= Site->Definition->BuildWorkUnits)
	{
		CompleteBuilding(*Site);
	}
}

void UConstructionSubsystem::CompleteBuilding(FConstructionSite& Site)
{
	Site.Phase = EConstructionPhase::Complete;
	if (Site.BuildingActor.IsValid())
	{
		Site.BuildingActor->ConstructionProgress = 1.f;
		Site.BuildingActor->OnStateChanged(EBuildingState::Complete);
	}

	if (UGameplayMessageSubsystem* Msg = UGameplayMessageSubsystem::GetIfValid(this))
	{
		Msg->BroadcastMessage(TAG_Message_Build_Completed, Site.BuildingId);
	}

	UE_LOG(LogConstruction, Log, TEXT("Building %d COMPLETE"), Site.BuildingId);
}

void UConstructionSubsystem::CancelBuilding(int32 BuildingId)
{
	FConstructionSite* Site = Sites.Find(BuildingId);
	if (!Site) return;

	UEconomySubsystem* Economy = GetWorld()->GetSubsystem<UEconomySubsystem>();
	if (Economy)
	{
		for (auto& [Resource, Handle] : Site->MaterialReservations)
		{
			Economy->ReleaseReservation(const_cast<FReservationHandle&>(Handle));
		}
	}

	UJobSubsystem* Jobs = GetWorld()->GetSubsystem<UJobSubsystem>();
	if (Jobs)
	{
		for (FJobHandle& Job : Site->ActiveJobs)
		{
			Jobs->CancelJob(Job);
		}
	}

	FreeCells(Site->GridOrigin, Site->Definition->FootprintCells);

	if (Site->BuildingActor.IsValid())
	{
		Site->BuildingActor->Destroy();
	}

	Sites.Remove(BuildingId);
	UE_LOG(LogConstruction, Log, TEXT("Building %d cancelled"), BuildingId);
}

void UConstructionSubsystem::OccupyCells(const FIntPoint& Origin, const FIntPoint& Footprint, int32 BuildingId)
{
	for (int32 X = 0; X < Footprint.X; ++X)
	{
		for (int32 Y = 0; Y < Footprint.Y; ++Y)
		{
			Grid.FindOrAdd(FIntPoint(Origin.X + X, Origin.Y + Y)).OccupiedByBuildingId = BuildingId;
		}
	}
}

void UConstructionSubsystem::FreeCells(const FIntPoint& Origin, const FIntPoint& Footprint)
{
	for (int32 X = 0; X < Footprint.X; ++X)
	{
		for (int32 Y = 0; Y < Footprint.Y; ++Y)
		{
			Grid.Remove(FIntPoint(Origin.X + X, Origin.Y + Y));
		}
	}
}

FConstructionSite* UConstructionSubsystem::GetSite(int32 Id)
{
	return Sites.Find(Id);
}
