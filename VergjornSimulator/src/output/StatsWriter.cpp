#include "StatsWriter.h"
#include <stdexcept>
#include <filesystem>

StatsWriter::StatsWriter(const std::string& path) {
    std::filesystem::create_directories(std::filesystem::path(path).parent_path());
    file_.open(path);
    if (!file_.is_open()) throw std::runtime_error("Cannot open stats file: " + path);
    writeHeader();
}

StatsWriter::~StatsWriter() {
    if (file_.is_open()) file_.close();
}

void StatsWriter::writeHeader() {
    file_ << "day,year,season,game_seconds,population,open_jobs,"
             "wood,bog_iron,charcoal,iron_ingot,tool_axe,weapon_sword,weapon_shield,fish,grain,"
             "avg_hunger,avg_warmth,avg_rest,avg_morale,"
             "working,idle\n";
}

void StatsWriter::write(const FrameStats& s) {
    file_ << s.day       << ','
          << s.year      << ','
          << toString(s.season) << ','
          << static_cast<long long>(s.gameSeconds) << ','
          << s.population << ','
          << s.openJobs   << ',';

    for (int i = 0; i < static_cast<int>(EResource::COUNT); ++i)
        file_ << s.resources[i] << ',';

    file_ << s.avgHunger  << ','
          << s.avgWarmth  << ','
          << s.avgRest    << ','
          << s.avgMorale  << ','
          << s.workingVillagers << ','
          << s.idleVillagers    << '\n';
}
