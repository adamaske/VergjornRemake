#include "TimeSubsystem.h"
#include "GameplayMessageSubsystem.h"
#include "VergjornTags.h"

DEFINE_LOG_CATEGORY(LogTime);

void UTimeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTime, Log, TEXT("TimeSubsystem ready. Day=%d Year=%d Season=%d"), CurrentDay, CurrentYear, (int32)CurrentSeason);
}

void UTimeSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UTimeSubsystem::AdvanceTime(float DeltaSeconds)
{
	const float GameDelta = DeltaSeconds * TimeScale;
	TotalGameSeconds   += GameDelta;
	GameSecondsThisDay += GameDelta;

	while (GameSecondsThisDay >= SecondsPerDay)
	{
		GameSecondsThisDay -= SecondsPerDay;
		OnNewDay();
	}
}

void UTimeSubsystem::OnNewDay()
{
	CurrentDay++;
	DayOfSeason++;

	UE_LOG(LogTime, Verbose, TEXT("New day %d (season day %d)"), CurrentDay, DayOfSeason);

	if (UGameplayMessageSubsystem* Msg = UGameplayMessageSubsystem::GetIfValid(this))
	{
		FNewDayMessage M;
		M.Day  = CurrentDay;
		M.Year = CurrentYear;
		Msg->BroadcastMessage(TAG_Message_Time_NewDay, M);
	}

	if (DayOfSeason >= DaysPerSeason)
	{
		DayOfSeason = 0;
		const EVergSeason Next = (EVergSeason)(((int32)CurrentSeason + 1) % SeasonsPerYear);
		OnSeasonChanged(Next);
		if (Next == EVergSeason::Spring) CurrentYear++;
	}
}

void UTimeSubsystem::OnSeasonChanged(EVergSeason New)
{
	CurrentSeason = New;
	UE_LOG(LogTime, Log, TEXT("Season -> %d  Year=%d"), (int32)CurrentSeason, CurrentYear);

	if (UGameplayMessageSubsystem* Msg = UGameplayMessageSubsystem::GetIfValid(this))
	{
		FSeasonChangedMessage M;
		M.NewSeason = SeasonToTag(New);
		M.Year      = CurrentYear;
		Msg->BroadcastMessage(TAG_Message_Time_SeasonChanged, M);
	}
}

FGameplayTag UTimeSubsystem::SeasonToTag(EVergSeason S) const
{
	switch (S)
	{
	case EVergSeason::Spring: return TAG_Season_Spring;
	case EVergSeason::Summer: return TAG_Season_Summer;
	case EVergSeason::Autumn: return TAG_Season_Autumn;
	case EVergSeason::Winter: return TAG_Season_Winter;
	default:                  return TAG_Season_Spring;
	}
}
