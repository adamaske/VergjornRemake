#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VergGameMode.generated.h"

class UTimeSubsystem;
class UPopulationSubsystem;

UCLASS()
class VERGJORN_API AVergGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AVergGameMode();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Number of villagers to spawn automatically when the level starts
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Vergjorn|Spawn")
	int32 StartingVillagerCount = 3;

private:
	UPROPERTY() TObjectPtr<UTimeSubsystem>     TimeSubsystem;
	UPROPERTY() TObjectPtr<UPopulationSubsystem> PopulationSubsystem;
};
