#include "NarrativeSubsystem.h"
#include "Engine/AssetManager.h"
#include "VergjornTags.h"

DEFINE_LOG_CATEGORY_STATIC(LogNarrative, Log, All);

void UNarrativeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LoadAllEventDefs();

	// Re-evaluate narrative eligibility every new in-game day
	if (UGameplayMessageSubsystem* Msg = UGameplayMessageSubsystem::GetIfValid(this))
	{
		// The new-day message carries an int32 day number; we only care that it fired
		NewDayHandle = Msg->RegisterListener<int32>(TAG_Message_Time_NewDay,
			[this](FGameplayTag, const int32&) { EvaluateAllEvents(); });
	}
}

void UNarrativeSubsystem::Deinitialize()
{
	NewDayHandle.Unregister();
	Super::Deinitialize();
}

// ── Trigger API ───────────────────────────────────────────────────────────────

bool UNarrativeSubsystem::TriggerEvent(FGameplayTag EventId)
{
	UNarrativeEventDefinition* Def = FindEventDef(EventId);
	if (!Def)
	{
		UE_LOG(LogNarrative, Warning, TEXT("TriggerEvent: no definition found for %s"), *EventId.ToString());
		return false;
	}
	if (Def->bOneShot && SeenEventIds.Contains(EventId))
	{
		UE_LOG(LogNarrative, Verbose, TEXT("TriggerEvent: %s already seen (bOneShot)"), *EventId.ToString());
		return false;
	}
	if (!MeetsConditions(Def))
	{
		UE_LOG(LogNarrative, Verbose, TEXT("TriggerEvent: conditions not met for %s"), *EventId.ToString());
		return false;
	}
	EnqueueEvent(Def);
	return true;
}

void UNarrativeSubsystem::EvaluateAllEvents()
{
	for (UNarrativeEventDefinition* Def : AllEventDefs)
	{
		if (!Def || Def->Lines.IsEmpty()) continue;
		if (Def->bOneShot && SeenEventIds.Contains(Def->EventId)) continue;
		if (!MeetsConditions(Def)) continue;

		// Don't double-queue an event already waiting
		const bool bAlreadyQueued = EventQueue.ContainsByPredicate(
			[&](const TObjectPtr<UNarrativeEventDefinition>& Q){ return Q && Q->EventId == Def->EventId; });
		if (bAlreadyQueued) continue;
		if (ActiveEvent && ActiveEvent->EventId == Def->EventId) continue;

		EnqueueEvent(Def);
	}
}

// ── Playback API ──────────────────────────────────────────────────────────────

void UNarrativeSubsystem::AdvanceLine()
{
	if (!ActiveEvent) return;

	++ActiveLineIndex;
	if (ActiveLineIndex < ActiveEvent->Lines.Num())
	{
		ShowCurrentLine();
	}
	else
	{
		OnEventFinished();
	}
}

void UNarrativeSubsystem::DismissCurrentEvent()
{
	if (!ActiveEvent) return;
	UE_LOG(LogNarrative, Log, TEXT("Narrative: event %s dismissed by player"), *ActiveEvent->EventId.ToString());
	ActiveEvent     = nullptr;
	ActiveLineIndex = INDEX_NONE;
	BeginNextEvent();
}

// ── Flag API ──────────────────────────────────────────────────────────────────

void UNarrativeSubsystem::SetFlag(FGameplayTag Flag, bool bValue)
{
	if (bValue)
		Flags.Add(Flag);
	else
		Flags.Remove(Flag);
}

// ── Save / Load ───────────────────────────────────────────────────────────────

FNarrativeProgressionState UNarrativeSubsystem::GetProgressionState() const
{
	FNarrativeProgressionState State;
	for (const FGameplayTag& Tag : SeenEventIds) State.SeenEventIds.Add(Tag);
	for (const FGameplayTag& Tag : Flags)        State.ActiveFlags.Add(Tag);
	return State;
}

void UNarrativeSubsystem::RestoreProgressionState(const FNarrativeProgressionState& State)
{
	SeenEventIds.Empty();
	Flags.Empty();
	for (const FGameplayTag& Tag : State.SeenEventIds) SeenEventIds.Add(Tag);
	for (const FGameplayTag& Tag : State.ActiveFlags)  Flags.Add(Tag);
}

// ── Private helpers ───────────────────────────────────────────────────────────

void UNarrativeSubsystem::LoadAllEventDefs()
{
	if (!UAssetManager::IsInitialized()) return;

	UAssetManager& AM = UAssetManager::Get();
	TArray<FPrimaryAssetId> AssetIds;
	AM.GetPrimaryAssetIdList(FPrimaryAssetType("NarrativeDef"), AssetIds);

	for (const FPrimaryAssetId& AssetId : AssetIds)
	{
		FSoftObjectPath Path = AM.GetPrimaryAssetPath(AssetId);
		if (!Path.IsValid()) continue;

		if (UNarrativeEventDefinition* Def = Cast<UNarrativeEventDefinition>(Path.TryLoad()))
		{
			AllEventDefs.Add(Def);
		}
	}
	UE_LOG(LogNarrative, Log, TEXT("Narrative: loaded %d event definitions"), AllEventDefs.Num());
}

UNarrativeEventDefinition* UNarrativeSubsystem::FindEventDef(FGameplayTag EventId) const
{
	for (UNarrativeEventDefinition* Def : AllEventDefs)
	{
		if (Def && Def->EventId == EventId) return Def;
	}
	return nullptr;
}

bool UNarrativeSubsystem::MeetsConditions(const UNarrativeEventDefinition* Def) const
{
	const FNarrativeConditions& C = Def->Conditions;

	// Required flags
	for (const FGameplayTag& Tag : C.RequiredFlags)
	{
		if (!Flags.Contains(Tag)) return false;
	}
	// Forbidden flags
	for (const FGameplayTag& Tag : C.ForbiddenFlags)
	{
		if (Flags.Contains(Tag)) return false;
	}
	// Required seen events
	for (const FGameplayTag& Tag : C.RequiredSeenEvents)
	{
		if (!SeenEventIds.Contains(Tag)) return false;
	}

	// Day / population checks are evaluated externally for now (subsystem has
	// no direct time/population reference here — callers should set flags when
	// those thresholds are crossed instead of relying on MinDay/MinPopulation
	// for the initial implementation).

	return true;
}

void UNarrativeSubsystem::EnqueueEvent(UNarrativeEventDefinition* Def)
{
	// Insert sorted by Priority descending so BeginNextEvent always picks index 0
	int32 InsertAt = EventQueue.Num();
	for (int32 i = 0; i < EventQueue.Num(); ++i)
	{
		if (EventQueue[i] && EventQueue[i]->Priority < Def->Priority)
		{
			InsertAt = i;
			break;
		}
	}
	EventQueue.Insert(Def, InsertAt);
	UE_LOG(LogNarrative, Log, TEXT("Narrative: enqueued %s (priority %d, queue depth %d)"),
		*Def->EventId.ToString(), Def->Priority, EventQueue.Num());

	if (!ActiveEvent)
	{
		BeginNextEvent();
	}
}

void UNarrativeSubsystem::BeginNextEvent()
{
	if (EventQueue.IsEmpty())
	{
		OnNarrativeQueueEmpty.Broadcast();
		return;
	}
	ActiveEvent     = EventQueue[0];
	ActiveLineIndex = 0;
	EventQueue.RemoveAt(0);

	UE_LOG(LogNarrative, Log, TEXT("Narrative: beginning event %s"), *ActiveEvent->EventId.ToString());
	ShowCurrentLine();
}

void UNarrativeSubsystem::ShowCurrentLine()
{
	if (!ActiveEvent || !ActiveEvent->Lines.IsValidIndex(ActiveLineIndex)) return;
	OnNarrativeLineDisplayed.Broadcast(ActiveEvent->Lines[ActiveLineIndex], ActiveEvent);
}

void UNarrativeSubsystem::OnEventFinished()
{
	if (!ActiveEvent) return;

	const FGameplayTag FinishedId = ActiveEvent->EventId;
	SeenEventIds.Add(FinishedId);

	// Apply consequences
	for (const FGameplayTag& Tag : ActiveEvent->Consequences.SetFlags)   Flags.Add(Tag);
	for (const FGameplayTag& Tag : ActiveEvent->Consequences.ClearFlags) Flags.Remove(Tag);

	UE_LOG(LogNarrative, Log, TEXT("Narrative: event %s completed"), *FinishedId.ToString());

	ActiveEvent     = nullptr;
	ActiveLineIndex = INDEX_NONE;

	OnNarrativeEventCompleted.Broadcast(FinishedId);
	BeginNextEvent();
}
