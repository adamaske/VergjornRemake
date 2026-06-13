#pragma once
#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "MassEntityQuery.h"
#include "VillagerSyncProcessor.generated.h"

// Runs in the UpdateWorldFromMass group (after Movement).
// Reads FTransformFragment and teleports each AVergVillagerVisual to match.
UCLASS()
class VERGJORNSIM_API UVillagerSyncProcessor : public UMassProcessor
{
	GENERATED_BODY()
public:
	UVillagerSyncProcessor();

protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

private:
	FMassEntityQuery EntityQuery;
};
