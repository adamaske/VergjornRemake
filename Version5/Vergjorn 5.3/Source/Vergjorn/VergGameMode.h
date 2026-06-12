#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VergGameMode.generated.h"

class UTimeSubsystem;

UCLASS()
class VERGJORN_API AVergGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AVergGameMode();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY() TObjectPtr<UTimeSubsystem> TimeSubsystem;
};
