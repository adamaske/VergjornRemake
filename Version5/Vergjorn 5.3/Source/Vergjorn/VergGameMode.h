#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VergGameMode.generated.h"

class UTimeSubsystem;
class UPopulationSubsystem;
class UNarrativeSubsystem;
class UVergDialogWidget;

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

	// Set this to a Blueprint child of UVergDialogWidget in your GameMode Blueprint
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Vergjorn|Narrative")
	TSubclassOf<UVergDialogWidget> DialogWidgetClass;

private:
	UPROPERTY() TObjectPtr<UTimeSubsystem>       TimeSubsystem;
	UPROPERTY() TObjectPtr<UPopulationSubsystem>  PopulationSubsystem;
	UPROPERTY() TObjectPtr<UNarrativeSubsystem>   NarrativeSubsystem;
	UPROPERTY() TObjectPtr<UVergDialogWidget>      DialogWidget;
};
