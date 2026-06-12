#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "EconomySubsystem.h"
#include "JobSubsystem.h"
#include "VergBuilding.h"
#include "ConstructionSubsystem.generated.h"

class UBuildingDefinition;

DECLARE_LOG_CATEGORY_EXTERN(LogConstruction, Log, All);

USTRUCT(BlueprintType)
struct VERGJORNBUILD_API FGridCell
{
	GENERATED_BODY()
	UPROPERTY() int32 OccupiedByBuildingId = INDEX_NONE;
	bool IsOccupied() const { return OccupiedByBuildingId != INDEX_NONE; }
};

UENUM(BlueprintType)
enum class EConstructionPhase : uint8
{
	Planned,
	MaterialsReserved,
	Hauling,
	Building,
	Complete,
};

USTRUCT(BlueprintType)
struct VERGJORNBUILD_API FConstructionSite
{
	GENERATED_BODY()

	UPROPERTY() int32                          BuildingId = INDEX_NONE;
	UPROPERTY() FIntPoint                      GridOrigin;
	UPROPERTY() TObjectPtr<UBuildingDefinition> Definition;
	UPROPERTY() TWeakObjectPtr<AVergBuilding>  BuildingActor;
	UPROPERTY() EConstructionPhase             Phase = EConstructionPhase::Planned;
	UPROPERTY() TMap<FGameplayTag, FReservationHandle> MaterialReservations;
	UPROPERTY() float                          WorkUnitsDone = 0.f;
	UPROPERTY() TArray<FJobHandle>             ActiveJobs;
};

UCLASS()
class VERGJORNBUILD_API UConstructionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	static constexpr float CellSize = 200.f;  // 200 UU ≈ 2 m

	FIntPoint WorldToGrid(const FVector& WorldPos) const;
	FVector   GridToWorld(const FIntPoint& GridPos, bool bCenterOfCell = true) const;

	bool CanPlace(const FIntPoint& Origin, const FIntPoint& Footprint) const;

	// Place a building; returns BuildingId or INDEX_NONE on failure
	int32 PlaceBuilding(UBuildingDefinition* Definition, const FIntPoint& GridOrigin);

	// Called when a haul job for this site completes
	void OnHaulJobCompleted(int32 BuildingId, FGameplayTag Resource, int32 Amount);

	// Called when builder job work advances
	void OnBuildJobCompleted(int32 BuildingId, float WorkUnits);

	void CancelBuilding(int32 BuildingId);

	const TMap<FIntPoint, FGridCell>& GetGrid()          const { return Grid; }
	FConstructionSite*                GetSite(int32 Id);

private:
	TMap<FIntPoint, FGridCell>    Grid;
	TMap<int32, FConstructionSite> Sites;
	int32 NextBuildingId = 1;

	void OccupyCells(const FIntPoint& Origin, const FIntPoint& Footprint, int32 BuildingId);
	void FreeCells(const FIntPoint& Origin, const FIntPoint& Footprint);

	void AdvanceToMaterialsPhase(FConstructionSite& Site);
	void AdvanceToBuildPhase(FConstructionSite& Site);
	void CompleteBuilding(FConstructionSite& Site);

	void PostHaulJobs(FConstructionSite& Site);
	void PostBuildJobs(FConstructionSite& Site);
};
