#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "TimeSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTime, Log, All);

USTRUCT(BlueprintType)
struct VERGJORNSIM_API FNewDayMessage
{
	GENERATED_BODY()
	UPROPERTY() int32 Day  = 0;
	UPROPERTY() int32 Year = 0;
};

USTRUCT(BlueprintType)
struct VERGJORNSIM_API FSeasonChangedMessage
{
	GENERATED_BODY()
	UPROPERTY() FGameplayTag NewSeason;
	UPROPERTY() int32 Year = 0;
};

UENUM(BlueprintType)
enum class EVergSeason : uint8
{
	Spring = 0,
	Summer,
	Autumn,
	Winter,
};

UCLASS()
class VERGJORNSIM_API UTimeSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Driven by GameMode Tick — consumes real DeltaSeconds scaled by TimeScale
	void AdvanceTime(float DeltaSeconds);

	UFUNCTION(BlueprintCallable) float        GetTimeScale()    const { return TimeScale; }
	UFUNCTION(BlueprintCallable) void         SetTimeScale(float S)   { TimeScale = FMath::Max(0.f, S); }
	UFUNCTION(BlueprintCallable) int32        GetCurrentDay()   const { return CurrentDay; }
	UFUNCTION(BlueprintCallable) int32        GetCurrentYear()  const { return CurrentYear; }
	UFUNCTION(BlueprintCallable) EVergSeason  GetCurrentSeason()const { return CurrentSeason; }
	UFUNCTION(BlueprintCallable) float        GetDayProgress()  const { return GameSecondsThisDay / SecondsPerDay; }

	double GetTotalGameSeconds() const { return TotalGameSeconds; }

	static constexpr float SecondsPerDay  = 600.f;  // 10 real-minutes = 1 game day
	static constexpr int32 DaysPerSeason  = 30;
	static constexpr int32 SeasonsPerYear = 4;

private:
	UPROPERTY(EditDefaultsOnly) float TimeScale = 1.f;

	float        GameSecondsThisDay = 0.f;
	int32        CurrentDay         = 1;
	int32        CurrentYear        = 1;
	EVergSeason  CurrentSeason      = EVergSeason::Spring;
	double       TotalGameSeconds   = 0.0;
	int32        DayOfSeason        = 0;

	void OnNewDay();
	void OnSeasonChanged(EVergSeason New);
	FGameplayTag SeasonToTag(EVergSeason S) const;
};
