#include "VillagerJobProcessor.h"
#include "MassExecutionContext.h"
#include "MassCommonFragments.h"
#include "VillagerFragments.h"
#include "JobSubsystem.h"

UVillagerJobProcessor::UVillagerJobProcessor()
{
	// Run in Behavior group (after Movement)
	ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Behavior;
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;
}

void UVillagerJobProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.AddRequirement<FVillagerIdentityFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.AddRequirement<FVillagerEquipmentFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.AddRequirement<FVillagerJobFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.RegisterWithProcessor(*this);
}

void UVillagerJobProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	const float DeltaTime = Context.GetDeltaTimeSeconds();
	if (DeltaTime <= 0.f) return;

	UJobSubsystem* Jobs = GetWorld()->GetSubsystem<UJobSubsystem>();
	if (!Jobs) return;

	EntityQuery.ForEachEntityChunk(EntityManager, Context, [&](FMassExecutionContext& ChunkCtx)
	{
		TConstArrayView<FTransformFragment>        Transforms = ChunkCtx.GetFragmentView<FTransformFragment>();
		TConstArrayView<FVillagerIdentityFragment>  Identities = ChunkCtx.GetFragmentView<FVillagerIdentityFragment>();
		TConstArrayView<FVillagerEquipmentFragment> Equipment  = ChunkCtx.GetFragmentView<FVillagerEquipmentFragment>();
		TArrayView<FVillagerJobFragment>            JobFrags   = ChunkCtx.GetMutableFragmentView<FVillagerJobFragment>();

		for (int32 i = 0; i < JobFrags.Num(); ++i)
		{
			FVillagerJobFragment&              JobFrag  = JobFrags[i];
			const FVillagerIdentityFragment&   Identity = Identities[i];
			const FVector                      Pos      = Transforms[i].GetTransform().GetLocation();

			// ── IDLE: search for a job ────────────────────────────────────────────
			if (JobFrag.CurrentJobId == INDEX_NONE)
			{
				FJobHandle Handle;
				if (Jobs->FindBestJob(Identity.Occupation, Pos, Handle))
				{
					if (Jobs->ClaimJob(Handle, Identity.VillagerId))
					{
						const FJobRecord* Record = Jobs->GetJob(Handle);
						JobFrag.CurrentJobId  = Handle.Id;
						JobFrag.JobType       = Record ? Record->JobType : FGameplayTag();
						JobFrag.JobLocation   = Record ? Record->Location : Pos;
						JobFrag.WorkProgress  = 0.f;
					}
				}
				continue;  // don't work on the frame we just claimed
			}

			// ── TRAVELLING: wait until we arrive ─────────────────────────────────
			const float DistSq = FVector::DistSquared2D(Pos, JobFrag.JobLocation);
			if (DistSq > ArrivalRadius * ArrivalRadius) continue;

			// ── WORKING: advance job ──────────────────────────────────────────────
			FJobHandle Handle;
			Handle.Id = JobFrag.CurrentJobId;

			FGuid CompletedBy;
			const float EffectiveRate = WorkUnitsPerSecond * Equipment[i].WorkEfficiencyMultiplier;
			const bool bDone = Jobs->AdvanceJob(Handle, EffectiveRate * DeltaTime, CompletedBy);

			if (bDone)
			{
				// UJobSubsystem already broadcast Message.Job.Completed.
				// Side-effects (wood deposit, building completion) are handled
				// by message listeners on AVergTreeNode / UConstructionSubsystem.
				JobFrag.CurrentJobId = INDEX_NONE;
				JobFrag.JobType      = FGameplayTag();
				JobFrag.WorkProgress = 0.f;
			}
		}
	});
}
