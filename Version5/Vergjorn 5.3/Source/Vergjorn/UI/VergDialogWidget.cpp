#include "VergDialogWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Engine/Texture2D.h"

void UVergDialogWidget::NativeConstruct()
{
	Super::NativeConstruct();

	NarrativeSubsystem = GetWorld()->GetSubsystem<UNarrativeSubsystem>();
	if (!NarrativeSubsystem) return;

	NarrativeSubsystem->OnNarrativeLineDisplayed.AddDynamic(this, &UVergDialogWidget::HandleLineDisplayed);
	NarrativeSubsystem->OnNarrativeQueueEmpty.AddDynamic(this, &UVergDialogWidget::HandleQueueEmpty);

	if (ContinueButton)
		ContinueButton->OnClicked.AddDynamic(this, &UVergDialogWidget::OnContinueClicked);

	if (DismissButton)
		DismissButton->OnClicked.AddDynamic(this, &UVergDialogWidget::OnDismissClicked);

	// Start hidden; activates when the first line arrives
	SetVisibility(ESlateVisibility::Collapsed);
}

void UVergDialogWidget::NativeDestruct()
{
	if (NarrativeSubsystem)
	{
		NarrativeSubsystem->OnNarrativeLineDisplayed.RemoveDynamic(this, &UVergDialogWidget::HandleLineDisplayed);
		NarrativeSubsystem->OnNarrativeQueueEmpty.RemoveDynamic(this, &UVergDialogWidget::HandleQueueEmpty);
	}
	Super::NativeDestruct();
}

void UVergDialogWidget::OnContinueClicked()
{
	if (NarrativeSubsystem)
		NarrativeSubsystem->AdvanceLine();
}

void UVergDialogWidget::OnDismissClicked()
{
	if (NarrativeSubsystem)
		NarrativeSubsystem->DismissCurrentEvent();
}

void UVergDialogWidget::HandleLineDisplayed(const FNarrativeLine& Line, const UNarrativeEventDefinition* EventDef)
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (SpeakerNameText && EventDef)
		SpeakerNameText->SetText(EventDef->SpeakerName);

	if (DialogLineText)
		DialogLineText->SetText(Line.Text);

	if (PortraitImage)
	{
		// Prefer the per-line portrait override, fall back to the event default
		TSoftObjectPtr<UTexture2D> PortraitPtr = Line.PortraitOverride.IsNull()
			? (EventDef ? EventDef->DefaultPortrait : TSoftObjectPtr<UTexture2D>())
			: Line.PortraitOverride;

		if (UTexture2D* Tex = PortraitPtr.Get())
		{
			PortraitImage->SetBrushFromTexture(Tex);
			PortraitImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			PortraitImage->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	BP_OnLineDisplayed(Line, EventDef);
}

void UVergDialogWidget::HandleQueueEmpty()
{
	SetVisibility(ESlateVisibility::Collapsed);
	BP_OnDialogClosed();
}
