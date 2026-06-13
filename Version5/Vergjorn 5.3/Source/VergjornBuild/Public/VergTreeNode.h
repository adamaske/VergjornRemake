#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayMessageSubsystem.h"
#include "JobSubsystem.h"
#include "VergTreeNode.generated.h"

class UStaticMeshComponent;

// A harvestable tree. Posts a Job.Chop when placed, deposits wood to the economy
// when the job completes (via Message.Job.Completed listener), then destroys itself.
UCLASS(BlueprintType, Blueprintable)
class VERGJORNBUILD_API AVergTreeNode : public AActor
{
	GENERATED_BODY()
public:
	AVergTreeNode();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Wood deposited to economy on chop completion
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tree")
	int32 WoodYield = 5;

	// Work units required to chop (matched to Job.WorkUnitsRequired)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tree")
	float ChopWorkUnits = 20.f;

	// Seconds before a new tree spawns in place (0 = no respawn)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tree")
	float RespawnDelay = 120.f;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	FJobHandle GetChopJobHandle() const { return ChopJobHandle; }

private:
	FJobHandle ChopJobHandle;
	FGameplayMessageListenerHandle MessageListenerHandle;

	void PostChopJob();
	void OnJobCompleted(FGameplayTag Channel, const FJobCompletedMessage& Message);
	void Chop();
};
