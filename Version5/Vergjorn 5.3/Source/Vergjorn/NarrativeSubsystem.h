#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "GameplayMessageSubsystem.h"
#include "NarrativeEventDefinition.h"
#include "NarrativeSubsystem.generated.h"

// Broadcast every time a new dialog line should be displayed
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNarrativeLineDisplayed,
	const FNarrativeLine&, Line,
	const UNarrativeEventDefinition*, EventDef);

// Broadcast when the last line of an event has been acknowledged
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNarrativeEventCompleted,
	FGameplayTag, EventId);

// Broadcast when the queue empties and no dialog is active
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNarrativeQueueEmpty);

// Snapshot of progression state for save/load integration
USTRUCT(BlueprintType)
struct FNarrativeProgressionState
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TArray<FGameplayTag> SeenEventIds;

	UPROPERTY(BlueprintReadOnly)
	TArray<FGameplayTag> ActiveFlags;
};

UCLASS()
class VERGJORN_API UNarrativeSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// ── Trigger API ───────────────────────────────────────────────────────────

	// Manually trigger an event by tag.  Returns false if conditions not met,
	// event already seen (and bOneShot), or event definition not found.
	UFUNCTION(BlueprintCallable, Category="Narrative")
	bool TriggerEvent(FGameplayTag EventId);

	// Re-evaluate every loaded definition against current conditions and enqueue
	// any that are newly eligible.  Call this from game systems on significant
	// state changes (new day, building completed, etc.).
	UFUNCTION(BlueprintCallable, Category="Narrative")
	void EvaluateAllEvents();

	// ── Playback API ──────────────────────────────────────────────────────────

	// Advance to the next line; called by the dialog widget's Continue button
	UFUNCTION(BlueprintCallable, Category="Narrative")
	void AdvanceLine();

	// Skip and discard the current event (e.g. player pressed ESC)
	UFUNCTION(BlueprintCallable, Category="Narrative")
	void DismissCurrentEvent();

	UFUNCTION(BlueprintPure, Category="Narrative")
	bool IsDialogActive() const { return ActiveEvent != nullptr; }

	// ── Flag API ──────────────────────────────────────────────────────────────

	UFUNCTION(BlueprintCallable, Category="Narrative")
	void SetFlag(FGameplayTag Flag, bool bValue);

	UFUNCTION(BlueprintPure, Category="Narrative")
	bool HasFlag(FGameplayTag Flag) const { return Flags.Contains(Flag); }

	UFUNCTION(BlueprintPure, Category="Narrative")
	bool HasSeenEvent(FGameplayTag EventId) const { return SeenEventIds.Contains(EventId); }

	// Allow a one-shot event to fire again (editor / cheat use)
	UFUNCTION(BlueprintCallable, Category="Narrative")
	void ResetEvent(FGameplayTag EventId) { SeenEventIds.Remove(EventId); }

	// ── Save / Load ───────────────────────────────────────────────────────────

	UFUNCTION(BlueprintCallable, Category="Narrative")
	FNarrativeProgressionState GetProgressionState() const;

	UFUNCTION(BlueprintCallable, Category="Narrative")
	void RestoreProgressionState(const FNarrativeProgressionState& State);

	// ── Delegates ─────────────────────────────────────────────────────────────

	UPROPERTY(BlueprintAssignable, Category="Narrative")
	FOnNarrativeLineDisplayed OnNarrativeLineDisplayed;

	UPROPERTY(BlueprintAssignable, Category="Narrative")
	FOnNarrativeEventCompleted OnNarrativeEventCompleted;

	UPROPERTY(BlueprintAssignable, Category="Narrative")
	FOnNarrativeQueueEmpty OnNarrativeQueueEmpty;

private:
	// ── Progression state ─────────────────────────────────────────────────────
	TSet<FGameplayTag> SeenEventIds;
	TSet<FGameplayTag> Flags;

	// ── Active playback ───────────────────────────────────────────────────────
	UPROPERTY()
	TObjectPtr<UNarrativeEventDefinition> ActiveEvent = nullptr;
	int32 ActiveLineIndex = INDEX_NONE;

	// Priority-sorted queue of pending events (highest priority first)
	UPROPERTY()
	TArray<TObjectPtr<UNarrativeEventDefinition>> EventQueue;

	// ── All loaded definitions ────────────────────────────────────────────────
	UPROPERTY()
	TArray<TObjectPtr<UNarrativeEventDefinition>> AllEventDefs;

	void LoadAllEventDefs();
	UNarrativeEventDefinition* FindEventDef(FGameplayTag EventId) const;

	bool MeetsConditions(const UNarrativeEventDefinition* Def) const;
	void EnqueueEvent(UNarrativeEventDefinition* Def);
	void BeginNextEvent();
	void ShowCurrentLine();
	void OnEventFinished();

	// Subscription to TAG_Message_Time_NewDay for auto-evaluation
	FGameplayMessageListenerHandle NewDayHandle;
};
