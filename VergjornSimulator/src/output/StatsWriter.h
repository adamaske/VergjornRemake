#pragma once
#include "core/Tags.h"
#include "core/Economy.h"
#include "core/SimTime.h"
#include <fstream>
#include <string>
#include <vector>

struct FrameStats {
    int   day;
    int   year;
    ESeason season;
    double gameSeconds;

    int   population;
    int   openJobs;

    // per-resource stored amounts (indexed by EResource)
    int   resources[static_cast<int>(EResource::COUNT)] = {};

    // need averages
    float avgHunger;
    float avgWarmth;
    float avgRest;
    float avgMorale;

    int   workingVillagers;   // those with an active job
    int   idleVillagers;
};

// Writes one CSV row per recorded frame to output/stats.csv
class StatsWriter {
public:
    explicit StatsWriter(const std::string& path);
    ~StatsWriter();

    void write(const FrameStats& s);

private:
    std::ofstream file_;
    void writeHeader();
};
