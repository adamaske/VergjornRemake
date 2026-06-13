#pragma once
#include "CoreMinimal.h"
#include "VergBuilding.h"
#include "GameplayMessageSubsystem.h"
#include "JobSubsystem.h"
#include "VergLoggingHut.generated.h"

class AVergTreeNode;

// The Logging Hut building.
// When construction completes it becomes operational: it scans for nearby trees
// and tracks how many Lumberjack workers are assigned (up to MaxWorkers from the
// BuildingDefinition). Workers claim Job.Chop jobs on their own via VillagerJobProcessor;
// the hut just provides the housing and tracks occupancy.
UCLASS(BlueprintType, Blueprintable)
class VERGJORNBUILD_API AVergLoggingHut : public AVergBuilding
{
	GENERATED_BODY()
public:
	AVergLoggingHut();
	virtual void Tick(float DeltaTime) override;
	virtual void OnStateChanged(EBuildingState NewState) override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

	// Radius to scan for harvestable trees (UU)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="LoggingHut")
	float WorkRadius = 3000.f;

	// Seconds between tree scans
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="LoggingHut")
	float ScanInterval = 10.f;

	// Returns how many worker slots are still open
	UFUNCTION(BlueprintCallable)
	int32 GetOpenWorkerSlots() const;

	// Called by PopulationSubsystem when assigning a lumberjack here
	bool AssignWorker(const FGuid& VillagerId);
	void UnassignWorker(const FGuid& VillagerId);

	UFUNCTION(BlueprintCallable)
	int32 GetAssignedWorkerCount() const { return AssignedWorkers.Num(); }

private:
	bool  bOperational      = false;
	float TimeSinceLastScan = 0.f;

	TArray<FGuid> AssignedWorkers;
	FGameplayMessageListenerHandle ChopCompletedHandle;

	void OnOperational();
	void ScanForTrees();
	int32 GetMaxWorkers() const;
	TArray<AVergTreeNode*> GetNearbyTrees() const;
};
