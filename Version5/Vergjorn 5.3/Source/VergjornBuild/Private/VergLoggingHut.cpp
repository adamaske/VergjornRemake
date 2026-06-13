#include "VergLoggingHut.h"
#include "VergTreeNode.h"
#include "BuildingDefinition.h"
#include "VergjornTags.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogLoggingHut, Log, All);

AVergLoggingHut::AVergLoggingHut()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AVergLoggingHut::OnStateChanged(EBuildingState NewState)
{
	Super::OnStateChanged(NewState);

	if (NewState == EBuildingState::Complete)
	{
		OnOperational();
	}
	else
	{
		bOperational = false;
		SetActorTickEnabled(false);
	}
}

void AVergLoggingHut::OnOperational()
{
	bOperational = true;
	SetActorTickEnabled(true);
	ScanForTrees();

	UE_LOG(LogLoggingHut, Log, TEXT("LoggingHut %d operational. MaxWorkers=%d WorkRadius=%.0f"),
		BuildingId, GetMaxWorkers(), WorkRadius);
}

void AVergLoggingHut::EndPlay(const EEndPlayReason::Type Reason)
{
	if (UGameplayMessageSubsystem* MsgSys = UGameplayMessageSubsystem::GetIfValid(this))
	{
		MsgSys->UnregisterListener(ChopCompletedHandle);
	}
	Super::EndPlay(Reason);
}

void AVergLoggingHut::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bOperational) return;

	TimeSinceLastScan += DeltaTime;
	if (TimeSinceLastScan >= ScanInterval)
	{
		TimeSinceLastScan = 0.f;
		ScanForTrees();
	}
}

void AVergLoggingHut::ScanForTrees()
{
	TArray<AVergTreeNode*> Trees = GetNearbyTrees();
	UE_LOG(LogLoggingHut, Verbose, TEXT("LoggingHut %d: %d trees in range, %d/%d workers assigned"),
		BuildingId, Trees.Num(), AssignedWorkers.Num(), GetMaxWorkers());
}

bool AVergLoggingHut::AssignWorker(const FGuid& VillagerId)
{
	if (AssignedWorkers.Num() >= GetMaxWorkers()) return false;
	if (AssignedWorkers.Contains(VillagerId)) return false;
	AssignedWorkers.Add(VillagerId);
	UE_LOG(LogLoggingHut, Log, TEXT("LoggingHut %d: worker assigned (%d/%d)"),
		BuildingId, AssignedWorkers.Num(), GetMaxWorkers());
	return true;
}

void AVergLoggingHut::UnassignWorker(const FGuid& VillagerId)
{
	AssignedWorkers.Remove(VillagerId);
}

int32 AVergLoggingHut::GetOpenWorkerSlots() const
{
	return FMath::Max(0, GetMaxWorkers() - AssignedWorkers.Num());
}

int32 AVergLoggingHut::GetMaxWorkers() const
{
	return Definition ? Definition->MaxWorkers : 2;
}

TArray<AVergTreeNode*> AVergLoggingHut::GetNearbyTrees() const
{
	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVergTreeNode::StaticClass(), Found);

	TArray<AVergTreeNode*> Nearby;
	const FVector HutLoc = GetActorLocation();
	for (AActor* A : Found)
	{
		if (FVector::Dist2D(A->GetActorLocation(), HutLoc) <= WorkRadius)
		{
			Nearby.Add(Cast<AVergTreeNode>(A));
		}
	}
	return Nearby;
}
