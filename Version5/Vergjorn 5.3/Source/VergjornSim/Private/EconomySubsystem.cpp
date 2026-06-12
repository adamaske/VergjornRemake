#include "EconomySubsystem.h"
#include "GameplayMessageSubsystem.h"
#include "VergjornTags.h"

DEFINE_LOG_CATEGORY(LogEconomy);

bool UEconomySubsystem::TryReserve(FGameplayTag Resource, int32 Amount, FReservationHandle& OutHandle)
{
	FResourceLedgerEntry& Entry = Ledger.FindOrAdd(Resource);
	if (Entry.Available() < Amount)
	{
		return false;
	}

	FReservationHandle Handle;
	Handle.Id       = NextReservationId++;
	Handle.Resource = Resource;
	Handle.Amount   = Amount;

	Entry.Reserved += Amount;
	ActiveReservations.Add(Handle.Id, Handle);
	OutHandle = Handle;

	BroadcastChanged(Resource);
	return true;
}

void UEconomySubsystem::CommitReservation(FReservationHandle& Handle)
{
	if (!Handle.IsValid()) return;

	if (ActiveReservations.Contains(Handle.Id))
	{
		FResourceLedgerEntry& Entry = Ledger.FindOrAdd(Handle.Resource);
		Entry.Stored   -= Handle.Amount;
		Entry.Reserved -= Handle.Amount;
		Entry.Stored   = FMath::Max(0, Entry.Stored);
		Entry.Reserved = FMath::Max(0, Entry.Reserved);
		ActiveReservations.Remove(Handle.Id);
		BroadcastChanged(Handle.Resource);
	}
	Handle.Invalidate();
}

void UEconomySubsystem::ReleaseReservation(FReservationHandle& Handle)
{
	if (!Handle.IsValid()) return;

	if (ActiveReservations.Contains(Handle.Id))
	{
		FResourceLedgerEntry& Entry = Ledger.FindOrAdd(Handle.Resource);
		Entry.Reserved -= Handle.Amount;
		Entry.Reserved  = FMath::Max(0, Entry.Reserved);
		ActiveReservations.Remove(Handle.Id);
		BroadcastChanged(Handle.Resource);
	}
	Handle.Invalidate();
}

void UEconomySubsystem::Deposit(FGameplayTag Resource, int32 Amount)
{
	if (Amount <= 0) return;
	FResourceLedgerEntry& Entry = Ledger.FindOrAdd(Resource);
	Entry.Stored += Amount;
	BroadcastChanged(Resource);
	UE_LOG(LogEconomy, Verbose, TEXT("Deposit %d %s -> Stored=%d"), Amount, *Resource.ToString(), Entry.Stored);
}

bool UEconomySubsystem::TryWithdraw(FGameplayTag Resource, int32 Amount)
{
	FResourceLedgerEntry* Entry = Ledger.Find(Resource);
	if (!Entry || Entry->Available() < Amount) return false;
	Entry->Stored -= Amount;
	BroadcastChanged(Resource);
	return true;
}

int32 UEconomySubsystem::GetAvailable(FGameplayTag Resource) const
{
	const FResourceLedgerEntry* E = Ledger.Find(Resource);
	return E ? E->Available() : 0;
}

int32 UEconomySubsystem::GetStored(FGameplayTag Resource) const
{
	const FResourceLedgerEntry* E = Ledger.Find(Resource);
	return E ? E->Stored : 0;
}

void UEconomySubsystem::MarkIncoming(FGameplayTag Resource, int32 Amount)
{
	Ledger.FindOrAdd(Resource).Incoming += Amount;
}

void UEconomySubsystem::UnmarkIncoming(FGameplayTag Resource, int32 Amount)
{
	FResourceLedgerEntry& E = Ledger.FindOrAdd(Resource);
	E.Incoming = FMath::Max(0, E.Incoming - Amount);
}

void UEconomySubsystem::BroadcastChanged(FGameplayTag Resource)
{
	if (UGameplayMessageSubsystem* MsgSys = UGameplayMessageSubsystem::GetIfValid(this))
	{
		FResourceChangedMessage Msg;
		Msg.Resource     = Resource;
		const FResourceLedgerEntry& E = Ledger[Resource];
		Msg.NewStored    = E.Stored;
		Msg.NewAvailable = E.Available();
		MsgSys->BroadcastMessage(TAG_Message_Economy_ResourceChanged, Msg);
	}
}
