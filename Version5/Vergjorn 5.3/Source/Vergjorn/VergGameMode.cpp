#include "VergGameMode.h"
#include "TimeSubsystem.h"
#include "PopulationSubsystem.h"
#include "VergjornTags.h"
#include "VergRTSCamera.h"

AVergGameMode::AVergGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	DefaultPawnClass = AVergRTSCamera::StaticClass();
}

void AVergGameMode::BeginPlay()
{
	Super::BeginPlay();

	TimeSubsystem      = GetWorld()->GetSubsystem<UTimeSubsystem>();
	PopulationSubsystem = GetWorld()->GetSubsystem<UPopulationSubsystem>();

	// Spawn the starting villagers spread around the origin
	if (PopulationSubsystem)
	{
		const float Spread = 300.f;
		for (int32 i = 0; i < StartingVillagerCount; ++i)
		{
			const float Angle = (2.f * UE_PI * i) / FMath::Max(StartingVillagerCount, 1);
			const FVector Loc(
				FMath::Cos(Angle) * Spread,
				FMath::Sin(Angle) * Spread,
				100.f   // above ground — fall to terrain on spawn
			);
			PopulationSubsystem->SpawnVillager(TAG_Class_Karl, TAG_Occupation_Lumberjack, Loc);
		}
	}
}

void AVergGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TimeSubsystem)
	{
		TimeSubsystem->AdvanceTime(DeltaTime);
	}
}
