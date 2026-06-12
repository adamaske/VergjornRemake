#include "VergGameMode.h"
#include "TimeSubsystem.h"
#include "VergRTSCamera.h"

AVergGameMode::AVergGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	DefaultPawnClass = AVergRTSCamera::StaticClass();
}

void AVergGameMode::BeginPlay()
{
	Super::BeginPlay();
	TimeSubsystem = GetWorld()->GetSubsystem<UTimeSubsystem>();
}

void AVergGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TimeSubsystem)
	{
		TimeSubsystem->AdvanceTime(DeltaTime);
	}
}
