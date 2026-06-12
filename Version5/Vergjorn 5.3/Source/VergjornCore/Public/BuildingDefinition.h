#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "BuildingDefinition.generated.h"

class UProductionRecipe;
class AVergBuilding;

UCLASS(BlueprintType)
class VERGJORNCORE_API UBuildingDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Building")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Building")
	FIntPoint FootprintCells = {2, 2};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Building")
	TMap<FGameplayTag, int32> ConstructionCost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Building")
	float BuildWorkUnits = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Building")
	TSoftClassPtr<AVergBuilding> BuildingClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Building")
	TObjectPtr<UProductionRecipe> Recipe;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Building")
	FGameplayTag RequiredOccupation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Building")
	int32 MaxWorkers = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Building")
	int32 HousingCapacity = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Building")
	FGameplayTag Category;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
