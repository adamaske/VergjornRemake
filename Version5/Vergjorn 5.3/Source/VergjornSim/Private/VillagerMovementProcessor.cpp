#include "VillagerMovementProcessor.h"
#include "MassExecutionContext.h"
#include "MassCommonFragments.h"
#include "VillagerFragments.h"

UVillagerMovementProcessor::UVillagerMovementProcessor()
{
	// Run before Behavior so entities arrive at the job site before work is ticked
	ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Movement;
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;
}

void UVillagerMovementProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FVillagerJobFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.AddConstSharedRequirement<FVillagerConfigSharedFragment>(EMassFragmentPresence::Optional);
	EntityQuery.RegisterWithProcessor(*this);
}

void UVillagerMovementProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	const float DeltaTime = Context.GetDeltaTimeSeconds();
	if (DeltaTime <= 0.f) return;

	EntityQuery.ForEachEntityChunk(EntityManager, Context, [&](FMassExecutionContext& ChunkCtx)
	{
		TArrayView<FTransformFragment>        Transforms = ChunkCtx.GetMutableFragmentView<FTransformFragment>();
		TConstArrayView<FVillagerJobFragment> Jobs       = ChunkCtx.GetFragmentView<FVillagerJobFragment>();

		const FVillagerConfigSharedFragment* Config = ChunkCtx.GetConstSharedFragmentPtr<FVillagerConfigSharedFragment>();
		const float Speed = Config ? Config->MovementSpeed : 150.f;

		for (int32 i = 0; i < Transforms.Num(); ++i)
		{
			const FVillagerJobFragment& Job = Jobs[i];
			if (Job.CurrentJobId == INDEX_NONE) continue;  // idle — nothing to move toward

			FTransform& T       = Transforms[i].GetMutableTransform();
			const FVector Pos   = T.GetLocation();
			const FVector Dest  = Job.JobLocation;
			const float   Dist  = FVector::Dist2D(Pos, Dest);

			if (Dist < 50.f) continue;  // already at site

			const FVector Dir    = (Dest - Pos).GetSafeNormal2D();
			const FVector NewPos = Pos + Dir * FMath::Min(Speed * DeltaTime, Dist);
			T.SetLocation(FVector(NewPos.X, NewPos.Y, Pos.Z));
		}
	});
}
