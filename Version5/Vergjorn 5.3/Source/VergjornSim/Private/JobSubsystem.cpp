#include "JobSubsystem.h"
#include "GameplayMessageSubsystem.h"
#include "VergjornTags.h"

DEFINE_LOG_CATEGORY(LogJob);

FJobHandle UJobSubsystem::PostJob(const FJobRecord& Job)
{
	FJobRecord Record  = Job;
	Record.Handle.Id   = NextJobId++;
	Record.Status      = EJobStatus::Open;
	Jobs.Add(Record.Handle.Id, Record);

	BroadcastJobPosted(Record);
	UE_LOG(LogJob, Verbose, TEXT("Job posted: %s (id=%d)"), *Record.JobType.ToString(), Record.Handle.Id);
	return Record.Handle;
}

bool UJobSubsystem::FindBestJob(FGameplayTag Occupation, const FVector& Location, FJobHandle& OutHandle)
{
	const FJobRecord* Best  = nullptr;
	float             BestScore = -1.f;

	for (auto& [Id, Record] : Jobs)
	{
		if (!Record.IsOpen()) continue;
		if (Record.RequiredOccupation.IsValid() && Record.RequiredOccupation != Occupation) continue;

		const float Dist  = FVector::Dist(Record.Location, Location);
		const float Score = Record.Priority * 1000.f - Dist;

		if (Score > BestScore || Best == nullptr)
		{
			BestScore = Score;
			Best      = &Record;
		}
	}

	if (Best)
	{
		OutHandle = Best->Handle;
		return true;
	}
	return false;
}

bool UJobSubsystem::ClaimJob(FJobHandle Handle, const FGuid& VillagerId)
{
	FJobRecord* Record = Jobs.Find(Handle.Id);
	if (!Record || !Record->IsOpen()) return false;

	Record->Status    = EJobStatus::Claimed;
	Record->ClaimedBy = VillagerId;
	return true;
}

bool UJobSubsystem::AdvanceJob(FJobHandle Handle, float WorkUnits, FGuid& OutCompletedBy)
{
	FJobRecord* Record = Jobs.Find(Handle.Id);
	if (!Record || Record->Status == EJobStatus::Completed) return false;

	Record->Status        = EJobStatus::InProgress;
	Record->WorkUnitsDone += WorkUnits;

	if (Record->WorkUnitsDone >= Record->WorkUnitsRequired)
	{
		Record->Status = EJobStatus::Completed;
		OutCompletedBy = Record->ClaimedBy;
		BroadcastJobCompleted(*Record);
		return true;
	}
	return false;
}

void UJobSubsystem::CancelJob(FJobHandle Handle)
{
	if (FJobRecord* Record = Jobs.Find(Handle.Id))
	{
		Record->Status = EJobStatus::Cancelled;
	}
}

FJobRecord* UJobSubsystem::GetJob(FJobHandle Handle)
{
	return Jobs.Find(Handle.Id);
}

int32 UJobSubsystem::GetOpenJobCount() const
{
	int32 Count = 0;
	for (auto& [Id, R] : Jobs)
	{
		if (R.IsOpen()) Count++;
	}
	return Count;
}

void UJobSubsystem::BroadcastJobPosted(const FJobRecord& Job)
{
	if (UGameplayMessageSubsystem* Msg = UGameplayMessageSubsystem::GetIfValid(this))
	{
		FJobPostedMessage M;
		M.Handle  = Job.Handle;
		M.JobType = Job.JobType;
		Msg->BroadcastMessage(TAG_Message_Job_Posted, M);
	}
}

void UJobSubsystem::BroadcastJobCompleted(const FJobRecord& Job)
{
	if (UGameplayMessageSubsystem* Msg = UGameplayMessageSubsystem::GetIfValid(this))
	{
		FJobCompletedMessage M;
		M.Handle      = Job.Handle;
		M.JobType     = Job.JobType;
		M.CompletedBy = Job.ClaimedBy;
		Msg->BroadcastMessage(TAG_Message_Job_Completed, M);
	}
}
