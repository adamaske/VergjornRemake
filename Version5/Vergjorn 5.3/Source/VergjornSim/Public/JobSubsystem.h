#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "EconomySubsystem.h"
#include "JobSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogJob, Log, All);

UENUM(BlueprintType)
enum class EJobStatus : uint8
{
	Open,
	Claimed,
	InProgress,
	Completed,
	Cancelled,
};

USTRUCT(BlueprintType)
struct VERGJORNSIM_API FJobHandle
{
	GENERATED_BODY()
	UPROPERTY() int32 Id = INDEX_NONE;

	bool IsValid()    const { return Id != INDEX_NONE; }
	void Invalidate()       { Id = INDEX_NONE; }
	bool operator==(const FJobHandle& O) const { return Id == O.Id; }
};

USTRUCT(BlueprintType)
struct VERGJORNSIM_API FJobRecord
{
	GENERATED_BODY()

	UPROPERTY() FJobHandle Handle;
	UPROPERTY() FGameplayTag JobType;
	UPROPERTY() FGameplayTag RequiredOccupation;
	UPROPERTY() FVector      Location = FVector::ZeroVector;
	UPROPERTY() int32        Priority = 0;
	UPROPERTY() float        WorkUnitsRequired = 10.f;
	UPROPERTY() float        WorkUnitsDone     = 0.f;
	UPROPERTY() FReservationHandle LinkedReservation;
	UPROPERTY() EJobStatus   Status    = EJobStatus::Open;
	UPROPERTY() FGuid        ClaimedBy;
	UPROPERTY() TWeakObjectPtr<AActor> TargetActor;

	bool IsOpen() const { return Status == EJobStatus::Open; }
};

USTRUCT()
struct VERGJORNSIM_API FJobPostedMessage
{
	GENERATED_BODY()
	UPROPERTY() FJobHandle   Handle;
	UPROPERTY() FGameplayTag JobType;
};

USTRUCT()
struct VERGJORNSIM_API FJobCompletedMessage
{
	GENERATED_BODY()
	UPROPERTY() FJobHandle   Handle;
	UPROPERTY() FGameplayTag JobType;
	UPROPERTY() FGuid        CompletedBy;
};

UCLASS()
class VERGJORNSIM_API UJobSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	// Post a new job; returns a handle
	FJobHandle PostJob(const FJobRecord& Job);

	// Find the best open job for a villager (occupation + distance scoring)
	bool FindBestJob(FGameplayTag Occupation, const FVector& Location, FJobHandle& OutHandle);

	// Claim a job for a specific villager
	bool ClaimJob(FJobHandle Handle, const FGuid& VillagerId);

	// Report work done; returns true when the job completes
	bool AdvanceJob(FJobHandle Handle, float WorkUnits, FGuid& OutCompletedBy);

	// Cancel a job (building cancelled, villager died, etc.)
	void CancelJob(FJobHandle Handle);

	FJobRecord* GetJob(FJobHandle Handle);

	int32 GetOpenJobCount() const;

private:
	TMap<int32, FJobRecord> Jobs;
	int32 NextJobId = 1;

	void BroadcastJobPosted(const FJobRecord& Job);
	void BroadcastJobCompleted(const FJobRecord& Job);
};
