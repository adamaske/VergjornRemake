#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "EconomySubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogEconomy, Log, All);

USTRUCT(BlueprintType)
struct VERGJORNSIM_API FResourceLedgerEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly) int32 Stored   = 0;
	UPROPERTY(BlueprintReadOnly) int32 Reserved = 0;
	UPROPERTY(BlueprintReadOnly) int32 Incoming = 0;

	int32 Available() const { return Stored - Reserved; }
};

USTRUCT(BlueprintType)
struct VERGJORNSIM_API FReservationHandle
{
	GENERATED_BODY()

	UPROPERTY() int32 Id = INDEX_NONE;
	UPROPERTY() FGameplayTag Resource;
	UPROPERTY() int32 Amount = 0;

	bool IsValid()    const { return Id != INDEX_NONE; }
	void Invalidate()       { Id = INDEX_NONE; }
};

USTRUCT()
struct VERGJORNSIM_API FResourceChangedMessage
{
	GENERATED_BODY()
	UPROPERTY() FGameplayTag Resource;
	UPROPERTY() int32 NewStored    = 0;
	UPROPERTY() int32 NewAvailable = 0;
};

UCLASS()
class VERGJORNSIM_API UEconomySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	// Reserve Amount; returns false if insufficient available
	bool TryReserve(FGameplayTag Resource, int32 Amount, FReservationHandle& OutHandle);

	// Consume the reserved amount (materials delivered/used)
	void CommitReservation(FReservationHandle& Handle);

	// Cancel a reservation (construction cancelled etc.)
	void ReleaseReservation(FReservationHandle& Handle);

	// Add resource to storage
	void Deposit(FGameplayTag Resource, int32 Amount);

	// Withdraw directly (production input consumption)
	bool TryWithdraw(FGameplayTag Resource, int32 Amount);

	// Stored - Reserved
	int32 GetAvailable(FGameplayTag Resource) const;
	int32 GetStored(FGameplayTag Resource)    const;

	// Track haul-jobs in flight
	void MarkIncoming(FGameplayTag Resource,   int32 Amount);
	void UnmarkIncoming(FGameplayTag Resource, int32 Amount);

	UFUNCTION(BlueprintCallable)
	const TMap<FGameplayTag, FResourceLedgerEntry>& GetAllEntries() const { return Ledger; }

private:
	TMap<FGameplayTag, FResourceLedgerEntry> Ledger;
	int32 NextReservationId = 1;
	TMap<int32, FReservationHandle> ActiveReservations;

	void BroadcastChanged(FGameplayTag Resource);
};
