#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "ResourceDefinition.generated.h"

UCLASS(BlueprintType)
class VERGJORNCORE_API UResourceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Resource")
	FGameplayTag ResourceTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Resource")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Resource")
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Resource")
	float StackSize = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Resource")
	bool bSpoils = false;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
