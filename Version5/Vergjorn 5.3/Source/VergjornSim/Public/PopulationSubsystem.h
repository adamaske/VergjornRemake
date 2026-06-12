#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "MassEntityHandle.h"
#include "PopulationSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPopulation, Log, All);

USTRUCT(BlueprintType)
struct VERGJORNSIM_API FHouseholdRecord
{
	GENERATED_BODY()
	UPROPERTY() int32         HouseholdId      = INDEX_NONE;
	UPROPERTY() TArray<FGuid> MemberIds;
	UPROPERTY() int32         DwellingBuildingId = INDEX_NONE;

	int32 Size() const { return MemberIds.Num(); }
};

USTRUCT(BlueprintType)
struct VERGJORNSIM_API FVillagerRecord
{
	GENERATED_BODY()
	UPROPERTY() FGuid            VillagerId;
	UPROPERTY() FMassEntityHandle EntityHandle;
	UPROPERTY() FGameplayTag     SocialClass;
	UPROPERTY() FGameplayTag     Occupation;
	UPROPERTY() int32            HouseholdId = INDEX_NONE;
};

USTRUCT()
struct VERGJORNSIM_API FVillagerSpawnedMessage
{
	GENERATED_BODY()
	UPROPERTY() FGuid        VillagerId;
	UPROPERTY() FGameplayTag SocialClass;
};

UCLASS()
class VERGJORNSIM_API UPopulationSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// Spawn a new villager Mass entity at Location
	FGuid SpawnVillager(FGameplayTag SocialClass, FGameplayTag Occupation, const FVector& Location);

	void DespawnVillager(const FGuid& VillagerId);

	bool AssignOccupation(const FGuid& VillagerId, FGameplayTag NewOccupation);

	// Called daily by TimeSubsystem listener to check for immigration
	void CheckImmigration(int32 FreeHousingSlots, float HappinessScore);

	int32 GetPopulationCount() const { return Villagers.Num(); }

	const TArray<FVillagerRecord>& GetAllVillagers() const { return Villagers; }

	FVillagerRecord* FindVillager(const FGuid& Id);

	// Exec command: Verg.SpawnVillagers <count>
	void DebugSpawnVillagers(int32 Count, const FVector& Location);

private:
	TArray<FVillagerRecord>       Villagers;
	TMap<int32, FHouseholdRecord> Households;
	int32 NextHouseholdId = 1;

	FMassEntityHandle SpawnMassEntity(FGameplayTag SocialClass, FGameplayTag Occupation, const FVector& Location);
};
