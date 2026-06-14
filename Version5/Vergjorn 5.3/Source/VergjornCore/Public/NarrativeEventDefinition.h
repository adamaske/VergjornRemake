#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "NarrativeTypes.h"
#include "NarrativeEventDefinition.generated.h"

// Data asset describing one complete narrative event (cutscene-free dialog).
// Create instances under /Game/Data/Narrative/ in the editor.
UCLASS(BlueprintType)
class VERGJORNCORE_API UNarrativeEventDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	// Unique tag for this event — used as a stable identifier in save data
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Narrative")
	FGameplayTag EventId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Narrative")
	FText SpeakerName;

	// Default portrait shown for all lines unless overridden per-line
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Narrative")
	TSoftObjectPtr<UTexture2D> DefaultPortrait;

	// Lines played in order; at least one is required for the event to fire
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Narrative")
	TArray<FNarrativeLine> Lines;

	// Optional message channel that auto-triggers an eligibility check when received.
	// Leave empty for scripted / manual trigger-only events.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Narrative|Trigger")
	FGameplayTag TriggerChannel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Narrative|Trigger")
	FNarrativeConditions Conditions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Narrative|Trigger")
	FNarrativeConsequences Consequences;

	// Higher-priority events preempt lower ones when multiple are eligible simultaneously
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Narrative|Trigger", meta=(ClampMin="0"))
	int32 Priority = 0;

	// If true, the event is skipped the second time its conditions are met
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Narrative|Trigger")
	bool bOneShot = true;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
