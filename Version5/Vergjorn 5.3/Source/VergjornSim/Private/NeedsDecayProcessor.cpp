#include "NeedsDecayProcessor.h"
#include "MassExecutionContext.h"
#include "VillagerFragments.h"

UNeedsDecayProcessor::UNeedsDecayProcessor()
{
	ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Behavior;
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;
}

void UNeedsDecayProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FVillagerNeedsFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddConstSharedRequirement<FVillagerConfigSharedFragment>(EMassFragmentPresence::Optional);
	EntityQuery.RegisterWithProcessor(*this);
}

void UNeedsDecayProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	const float DeltaTime = Context.GetDeltaTimeSeconds();
	if (DeltaTime <= 0.f) return;

	EntityQuery.ForEachEntityChunk(EntityManager, Context, [&](FMassExecutionContext& ChunkCtx)
	{
		TArrayView<FVillagerNeedsFragment> NeedsArray = ChunkCtx.GetMutableFragmentView<FVillagerNeedsFragment>();

		const FVillagerConfigSharedFragment* Config = ChunkCtx.GetConstSharedFragmentPtr<FVillagerConfigSharedFragment>();
		const float DecayRate = Config ? Config->NeedsDecayRate : 0.005f;

		for (FVillagerNeedsFragment& Needs : NeedsArray)
		{
			Needs.Hunger = FMath::Max(0.f, Needs.Hunger - DecayRate          * DeltaTime);
			Needs.Warmth = FMath::Max(0.f, Needs.Warmth - DecayRate * 0.5f  * DeltaTime);
			Needs.Rest   = FMath::Max(0.f, Needs.Rest   - DecayRate * 0.8f  * DeltaTime);
		}
	});
}
