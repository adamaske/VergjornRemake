#pragma once
#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "VillagerMovementProcessor.generated.h"

// Moves villager entities toward their current job location each frame.
// Runs before VillagerJobProcessor so villagers arrive before work starts.
UCLASS()
class VERGJORNSIM_API UVillagerMovementProcessor : public UMassProcessor
{
	GENERATED_BODY()
public:
	UVillagerMovementProcessor();

protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

private:
	FMassEntityQuery EntityQuery;
};
