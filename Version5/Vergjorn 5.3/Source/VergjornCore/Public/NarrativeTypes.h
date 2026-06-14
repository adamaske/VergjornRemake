#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/Texture2D.h"
#include "NarrativeTypes.generated.h"

UENUM(BlueprintType)
enum class ENarrativeEmotion : uint8
{
	Neutral,
	Happy,
	Sad,
	Angry,
	Fearful,
	Surprised,
};

// A single line of dialog spoken in sequence
USTRUCT(BlueprintType)
struct VERGJORNCORE_API FNarrativeLine
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Narrative")
	FText Text;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Narrative")
	ENarrativeEmotion Emotion = ENarrativeEmotion::Neutral;

	// If set, overrides the event's DefaultPortrait for this line only
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Narrative")
	TSoftObjectPtr<UTexture2D> PortraitOverride;
};

// All conditions must pass for a narrative event to become eligible
USTRUCT(BlueprintType)
struct VERGJORNCORE_API FNarrativeConditions
{
	GENERATED_BODY()

	// All of these flags must currently be set on the NarrativeSubsystem
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Narrative")
	FGameplayTagContainer RequiredFlags;

	// None of these flags may be set
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Narrative")
	FGameplayTagContainer ForbiddenFlags;

	// All of these event IDs must have already been seen
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Narrative")
	FGameplayTagContainer RequiredSeenEvents;

	// Minimum in-game day (0 = always eligible)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Narrative", meta=(ClampMin="0"))
	int32 MinDay = 0;

	// Minimum total population (0 = always eligible)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Narrative", meta=(ClampMin="0"))
	int32 MinPopulation = 0;
};

// Side-effects applied once an event's last line has been acknowledged
USTRUCT(BlueprintType)
struct VERGJORNCORE_API FNarrativeConsequences
{
	GENERATED_BODY()

	// Flags to add on the NarrativeSubsystem after the event completes
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Narrative")
	FGameplayTagContainer SetFlags;

	// Flags to remove after the event completes
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Narrative")
	FGameplayTagContainer ClearFlags;
};
