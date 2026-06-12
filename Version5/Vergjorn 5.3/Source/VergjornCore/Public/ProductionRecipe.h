#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "ProductionRecipe.generated.h"

UCLASS(BlueprintType)
class VERGJORNCORE_API UProductionRecipe : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Recipe")
	TMap<FGameplayTag, int32> Inputs;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Recipe")
	TMap<FGameplayTag, int32> Outputs;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Recipe")
	float WorkSeconds = 20.f;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
