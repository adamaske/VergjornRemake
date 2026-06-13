#include "VillagerSyncProcessor.h"
#include "VillagerFragments.h"
#include "VergVillagerVisual.h"
#include "MassCommonFragments.h"
#include "MassExecutionContext.h"

UVillagerSyncProcessor::UVillagerSyncProcessor()
{
	// Run after Behavior and Movement have updated FTransformFragment
	ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::UpdateWorldFromMass;
	ExecutionFlags = (int32)(EProcessorExecutionFlags::All);
}

void UVillagerSyncProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.AddRequirement<FVillagerVisualFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.RegisterWithProcessor(*this);
}

void UVillagerSyncProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	EntityQuery.ForEachEntityChunk(EntityManager, Context,
		[](FMassExecutionContext& Ctx)
		{
			TConstArrayView<FTransformFragment>     Transforms = Ctx.GetFragmentView<FTransformFragment>();
			TConstArrayView<FVillagerVisualFragment> Visuals   = Ctx.GetFragmentView<FVillagerVisualFragment>();

			for (int32 i = 0; i < Ctx.GetNumEntities(); ++i)
			{
				if (AVergVillagerVisual* Visual = Visuals[i].VisualActor.Get())
				{
					// TeleportPhysics: skip collision sweeps — position is authoritative from simulation
					Visual->SetActorTransform(
						Transforms[i].GetTransform(),
						/*bSweep=*/false,
						/*OutSweepHitResult=*/nullptr,
						ETeleportType::TeleportPhysics);
				}
			}
		});
}
