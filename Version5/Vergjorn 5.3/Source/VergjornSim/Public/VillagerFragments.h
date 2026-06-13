#pragma once
#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "GameplayTagContainer.h"
#include "VillagerFragments.generated.h"

class AVergVillagerVisual;

// Unique identity, social class, occupation, household
USTRUCT()
struct VERGJORNSIM_API FVillagerIdentityFragment : public FMassFragment
{
	GENERATED_BODY()

	UPROPERTY()
	FGuid VillagerId;

	UPROPERTY()
	FGameplayTag SocialClass;

	UPROPERTY()
	FGameplayTag Occupation;

	UPROPERTY()
	int32 HouseholdId = INDEX_NONE;

	UPROPERTY()
	FText Name;
};

// Needs that decay with game time
USTRUCT()
struct VERGJORNSIM_API FVillagerNeedsFragment : public FMassFragment
{
	GENERATED_BODY()

	UPROPERTY()
	float Hunger = 1.f;   // 1 = full; decays toward 0

	UPROPERTY()
	float Warmth = 1.f;

	UPROPERTY()
	float Rest = 1.f;

	UPROPERTY()
	float Morale = 0.7f;
};

// Current job assignment
USTRUCT()
struct VERGJORNSIM_API FVillagerJobFragment : public FMassFragment
{
	GENERATED_BODY()

	UPROPERTY()
	int32 CurrentJobId = INDEX_NONE;

	UPROPERTY()
	FGameplayTag JobType;

	UPROPERTY()
	float WorkProgress = 0.f;

	UPROPERTY()
	FVector JobLocation = FVector::ZeroVector;
};

// Resources being physically carried by this villager
USTRUCT()
struct VERGJORNSIM_API FCarriedResourceFragment : public FMassFragment
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayTag Resource;

	UPROPERTY()
	int32 Amount = 0;
};

// Equipment carried / worn by this villager.
// WorkEfficiencyMultiplier is cached from UEquipmentDefinition at assignment time
// so the job processor never touches the Asset Manager in its hot path.
USTRUCT()
struct VERGJORNSIM_API FVillagerEquipmentFragment : public FMassFragment
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayTag EquippedTool;    // e.g. TAG_Equipment_Tool_Axe

	UPROPERTY()
	FGameplayTag EquippedWeapon;  // e.g. TAG_Equipment_Weapon_Sword

	UPROPERTY()
	FGameplayTag EquippedArmor;   // future

	// Set by PopulationSubsystem (or EquipmentSubsystem) whenever equipment changes.
	// Default 1.0 = bare-handed.
	UPROPERTY()
	float WorkEfficiencyMultiplier = 1.0f;
};

// Scene actor that renders this villager — written once at spawn, read each frame by VillagerSyncProcessor
USTRUCT()
struct VERGJORNSIM_API FVillagerVisualFragment : public FMassFragment
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<AVergVillagerVisual> VisualActor = nullptr;
};

// Shared config fragment — same instance shared by all villagers of the same archetype
USTRUCT()
struct VERGJORNSIM_API FVillagerConfigSharedFragment : public FMassSharedFragment
{
	GENERATED_BODY()

	UPROPERTY()
	float NeedsDecayRate = 0.005f;  // per real second

	UPROPERTY()
	float MovementSpeed = 150.f;
};

