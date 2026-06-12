#pragma once
#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "NeedsDecayProcessor.generated.h"

// Decays Hunger/Warmth/Rest on all villager entities each frame (time-sliced via Mass)
UCLASS()
class VERGJORNSIM_API UNeedsDecayProcessor : public UMassProcessor
{
	GENERATED_BODY()
public:
	UNeedsDecayProcessor();

protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

private:
	FMassEntityQuery EntityQuery;
};
