#pragma once
#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "VillagerJobProcessor.generated.h"

// Handles the full villager job lifecycle per entity:
//   Idle   → find + claim best available job
//   Travelling → wait (VillagerMovementProcessor handles movement)
//   Working → advance job work units; UJobSubsystem broadcasts Message.Job.Completed on finish
UCLASS()
class VERGJORNSIM_API UVillagerJobProcessor : public UMassProcessor
{
	GENERATED_BODY()
public:
	UVillagerJobProcessor();

protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

private:
	FMassEntityQuery EntityQuery;

	// Work done per real second while at the job site
	static constexpr float WorkUnitsPerSecond = 5.f;

	// How close a villager must be to start working (UU)
	static constexpr float ArrivalRadius = 100.f;
};
