#pragma once
#include "Tags.h"
#include <functional>
#include <cstdint>

// Game clock. Driven by calling advance() each simulation tick.
// 1 game day = SecondsPerDay real seconds * timeScale.
class SimTime {
public:
    static constexpr float SecondsPerDay  = 600.f;
    static constexpr int   DaysPerSeason  = 30;
    static constexpr int   SeasonsPerYear = 4;

    // Callbacks
    std::function<void(int day, int year)> onNewDay;
    std::function<void(ESeason season, int year)> onSeasonChanged;

    void advance(float realDeltaSeconds);

    float    timeScale()        const { return scale_; }
    void     setTimeScale(float s)    { scale_ = (s > 0.f ? s : 0.f); }
    int      day()              const { return day_; }
    int      year()             const { return year_; }
    ESeason  season()           const { return season_; }
    float    dayProgress()      const { return secondsThisDay_ / SecondsPerDay; }
    double   totalGameSeconds() const { return totalGameSeconds_; }

    // How many total in-game days have passed
    int totalDays() const { return (year_ - 1) * DaysPerSeason * SeasonsPerYear + seasonDay_ + (day_ - 1); }

private:
    float   scale_           = 1.f;
    float   secondsThisDay_  = 0.f;
    int     day_             = 1;
    int     year_            = 1;
    int     seasonDay_       = 0;      // day within current season
    ESeason season_          = ESeason::Spring;
    double  totalGameSeconds_= 0.0;

    void onDayEnd();
    void onSeasonEnd();
};
