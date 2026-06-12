#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "OccupationDefinition.generated.h"

class UStateTree;

UCLASS(BlueprintType)
class VERGJORNCORE_API UOccupationDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Occupation")
	FGameplayTag OccupationTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Occupation")
	FGameplayTagContainer AllowedClasses;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Occupation")
	TSoftObjectPtr<UStateTree> BehaviorTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Occupation")
	TMap<FGameplayTag, int32> RequiredEquipment;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
