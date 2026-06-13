#include "PopulationSubsystem.h"
#include "VergVillagerVisual.h"
#include "VillagerFragments.h"
#include "MassEntitySubsystem.h"
#include "MassCommonFragments.h"
#include "GameplayMessageSubsystem.h"
#include "VergjornTags.h"

// ── Placeholder tool assignment ───────────────────────────────────────────────
// Maps each occupation to its starter tool tag and efficiency multiplier.
// Remove this once EquipmentSubsystem handles distribution from the smithy.
static void AssignStarterEquipment(FVillagerEquipmentFragment& Equip, FGameplayTag Occupation)
{
	if      (Occupation == TAG_Occupation_Lumberjack) { Equip.EquippedTool = TAG_Equipment_Tool_Axe;        Equip.WorkEfficiencyMultiplier = 1.5f; }
	else if (Occupation == TAG_Occupation_BogWorker)  { Equip.EquippedTool = TAG_Equipment_Tool_Pickaxe;    Equip.WorkEfficiencyMultiplier = 1.5f; }
	else if (Occupation == TAG_Occupation_Fisher)     { Equip.EquippedTool = TAG_Equipment_Tool_FishingRod; Equip.WorkEfficiencyMultiplier = 1.4f; }
	else if (Occupation == TAG_Occupation_Farmer)     { Equip.EquippedTool = TAG_Equipment_Tool_Scythe;     Equip.WorkEfficiencyMultiplier = 1.5f; }
	else if (Occupation == TAG_Occupation_Blacksmith ||
	         Occupation == TAG_Occupation_Builder)    { Equip.EquippedTool = TAG_Equipment_Tool_Hammer;     Equip.WorkEfficiencyMultiplier = 1.3f; }
	// Haulers, Vikings, etc.: no tool bonus — multiplier stays 1.0
}

DEFINE_LOG_CATEGORY(LogPopulation);

void UPopulationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Collection.InitializeDependency<UMassEntitySubsystem>();
}

FGuid UPopulationSubsystem::SpawnVillager(FGameplayTag SocialClass, FGameplayTag Occupation, const FVector& Location)
{
	FVillagerRecord Record;
	Record.VillagerId   = FGuid::NewGuid();
	Record.SocialClass  = SocialClass;
	Record.Occupation   = Occupation;
	Record.EntityHandle = SpawnMassEntity(SocialClass, Occupation, Location);

	// Spawn visual actor and bind it to the Mass entity
	if (Record.EntityHandle.IsValid())
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AVergVillagerVisual* Visual = GetWorld()->SpawnActor<AVergVillagerVisual>(Location, FRotator::ZeroRotator, Params);
		if (Visual)
		{
			Visual->EntityHandle = Record.EntityHandle;

			if (UMassEntitySubsystem* MassES = GetWorld()->GetSubsystem<UMassEntitySubsystem>())
			{
				FMassEntityManager& EM = MassES->GetMutableEntityManager();
				EM.GetFragmentDataChecked<FVillagerVisualFragment>(Record.EntityHandle).VisualActor = Visual;
			}
		}
	}

	Villagers.Add(Record);

	if (UGameplayMessageSubsystem* Msg = UGameplayMessageSubsystem::GetIfValid(this))
	{
		FVillagerSpawnedMessage M;
		M.VillagerId  = Record.VillagerId;
		M.SocialClass = SocialClass;
		Msg->BroadcastMessage(TAG_Message_Population_VillagerSpawned, M);
	}

	UE_LOG(LogPopulation, Log, TEXT("Villager spawned: %s  Class=%s  Occ=%s"),
		*Record.VillagerId.ToString(), *SocialClass.ToString(), *Occupation.ToString());

	return Record.VillagerId;
}

void UPopulationSubsystem::DespawnVillager(const FGuid& VillagerId)
{
	const int32 Idx = Villagers.IndexOfByPredicate([&](const FVillagerRecord& R){ return R.VillagerId == VillagerId; });
	if (Idx == INDEX_NONE) return;

	const FVillagerRecord& Record = Villagers[Idx];

	if (UMassEntitySubsystem* MassES = GetWorld()->GetSubsystem<UMassEntitySubsystem>())
	{
		FMassEntityManager& EM = MassES->GetMutableEntityManager();
		if (EM.IsEntityActive(Record.EntityHandle))
		{
			// Destroy the visual actor before removing the entity
			FVillagerVisualFragment& VisualFrag = EM.GetFragmentDataChecked<FVillagerVisualFragment>(Record.EntityHandle);
			if (VisualFrag.VisualActor.Get())
			{
				VisualFrag.VisualActor->Destroy();
				VisualFrag.VisualActor = nullptr;
			}

			EM.DestroyEntity(Record.EntityHandle);
		}
	}

	Villagers.RemoveAtSwap(Idx);
}

bool UPopulationSubsystem::AssignOccupation(const FGuid& VillagerId, FGameplayTag NewOccupation)
{
	FVillagerRecord* Record = FindVillager(VillagerId);
	if (!Record) return false;
	Record->Occupation = NewOccupation;
	UE_LOG(LogPopulation, Log, TEXT("Villager %s -> occupation %s"), *VillagerId.ToString(), *NewOccupation.ToString());
	return true;
}

void UPopulationSubsystem::CheckImmigration(int32 FreeHousingSlots, float HappinessScore)
{
	if (FreeHousingSlots <= 0 || HappinessScore < 0.5f) return;

	const int32 ToSpawn = FMath::Min(FreeHousingSlots, 2);
	for (int32 i = 0; i < ToSpawn; i++)
	{
		SpawnVillager(TAG_Class_Karl, TAG_Occupation_Hauler, FVector::ZeroVector);
	}
	UE_LOG(LogPopulation, Log, TEXT("Immigration: %d new Karl villagers"), ToSpawn);
}

FVillagerRecord* UPopulationSubsystem::FindVillager(const FGuid& Id)
{
	return Villagers.FindByPredicate([&](const FVillagerRecord& R){ return R.VillagerId == Id; });
}

void UPopulationSubsystem::DebugSpawnVillagers(int32 Count, const FVector& Location)
{
	for (int32 i = 0; i < Count; i++)
	{
		const FVector Offset(FMath::RandRange(-500, 500), FMath::RandRange(-500, 500), 0);
		SpawnVillager(TAG_Class_Karl, TAG_Occupation_Hauler, Location + Offset);
	}
	UE_LOG(LogPopulation, Log, TEXT("Debug: spawned %d villagers"), Count);
}

FMassEntityHandle UPopulationSubsystem::SpawnMassEntity(FGameplayTag SocialClass, FGameplayTag Occupation, const FVector& Location)
{
	UMassEntitySubsystem* MassES = GetWorld()->GetSubsystem<UMassEntitySubsystem>();
	if (!MassES) return FMassEntityHandle();

	FMassEntityManager& EM = MassES->GetMutableEntityManager();

	FMassArchetypeHandle Archetype = EM.CreateArchetype(
		TArray<const UScriptStruct*>{
			FTransformFragment::StaticStruct(),
			FVillagerIdentityFragment::StaticStruct(),
			FVillagerNeedsFragment::StaticStruct(),
			FVillagerJobFragment::StaticStruct(),
			FCarriedResourceFragment::StaticStruct(),
			FVillagerEquipmentFragment::StaticStruct(),
			FVillagerVisualFragment::StaticStruct(),
		}
	);

	FMassEntityHandle Entity = EM.CreateEntity(Archetype);

	// Position
	EM.GetFragmentDataChecked<FTransformFragment>(Entity).SetTransform(FTransform(Location));

	// Identity
	FVillagerIdentityFragment& Identity = EM.GetFragmentDataChecked<FVillagerIdentityFragment>(Entity);
	Identity.VillagerId  = FGuid::NewGuid();
	Identity.SocialClass = SocialClass;
	Identity.Occupation  = Occupation;

	// Starter equipment — placeholder until EquipmentSubsystem is implemented
	AssignStarterEquipment(EM.GetFragmentDataChecked<FVillagerEquipmentFragment>(Entity), Occupation);

	return Entity;
}
