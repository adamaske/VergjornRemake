#include "SimTime.h"

void SimTime::advance(float realDeltaSeconds) {
    float gameDelta = realDeltaSeconds * scale_;
    totalGameSeconds_ += gameDelta;
    secondsThisDay_   += gameDelta;

    while (secondsThisDay_ >= SecondsPerDay) {
        secondsThisDay_ -= SecondsPerDay;
        onDayEnd();
    }
}

void SimTime::onDayEnd() {
    day_++;
    seasonDay_++;

    if (onNewDay) onNewDay(day_, year_);

    if (seasonDay_ >= DaysPerSeason) {
        seasonDay_ = 0;
        onSeasonEnd();
    }
}

void SimTime::onSeasonEnd() {
    int next = (static_cast<int>(season_) + 1) % SeasonsPerYear;
    season_ = static_cast<ESeason>(next);

    if (season_ == ESeason::Spring) year_++;

    if (onSeasonChanged) onSeasonChanged(season_, year_);
}
