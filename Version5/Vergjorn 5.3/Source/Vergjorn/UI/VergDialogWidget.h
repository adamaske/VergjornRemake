#pragma once
#include "CoreMinimal.h"
#include "V_ActivatableWidget.h"
#include "NarrativeSubsystem.h"
#include "VergDialogWidget.generated.h"

class UTextBlock;
class UImage;
class UButton;
class UNarrativeSubsystem;

// Widget that renders the active narrative event.
// Bind this in Blueprint: portrait Image, speaker TextBlock, line TextBlock, Continue Button.
// The widget self-activates when a line is broadcast and self-deactivates when the queue empties.
UCLASS(Blueprintable, BlueprintType)
class VERGJORN_API UVergDialogWidget : public UV_ActivatableWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// Called by the Continue button
	UFUNCTION(BlueprintCallable, Category="Narrative")
	void OnContinueClicked();

	// Called by a Skip/Dismiss button (optional)
	UFUNCTION(BlueprintCallable, Category="Narrative")
	void OnDismissClicked();

protected:
	// Override in Blueprint to animate the panel in/out, update portrait texture, etc.
	UFUNCTION(BlueprintImplementableEvent, Category="Narrative")
	void BP_OnLineDisplayed(const FNarrativeLine& Line, const UNarrativeEventDefinition* EventDef);

	UFUNCTION(BlueprintImplementableEvent, Category="Narrative")
	void BP_OnDialogClosed();

	// Bound widgets — set these in the Blueprint graph's NativeConstruct or via meta=(BindWidget)
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> SpeakerNameText;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> DialogLineText;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UImage> PortraitImage;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UButton> ContinueButton;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UButton> DismissButton;

private:
	UFUNCTION()
	void HandleLineDisplayed(const FNarrativeLine& Line, const UNarrativeEventDefinition* EventDef);

	UFUNCTION()
	void HandleQueueEmpty();

	UPROPERTY()
	TObjectPtr<UNarrativeSubsystem> NarrativeSubsystem;
};
