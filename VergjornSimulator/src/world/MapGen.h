#pragma once
#include "MapZone.h"
#include <vector>
#include <cstdint>
#include <random>

struct MapConfig {
    float    worldRadius       = 2000.f;
    int      numForestPatches  = 4;
    int      treesPerForest    = 12;
    int      numBogPatches     = 2;
    int      oresPerBog        = 6;
    int      numFishingGrounds = 2;
    int      fishSpotsPerGround= 5;
    uint32_t seed              = 42;
};

struct GeneratedMap {
    std::vector<MapZone> zones;
    std::vector<Vec2>    treePositions;
    std::vector<Vec2>    orePositions;
    std::vector<Vec2>    fishPositions;

    // River: y = riverSlope * x + riverOffset
    float riverSlope  = 0.f;
    float riverOffset = 0.f;

    float    worldRadius = 2000.f;
    uint32_t seed        = 42;
};

class MapGen {
public:
    GeneratedMap generate(const MapConfig& cfg);

private:
    std::mt19937 rng_;
    int          nextZoneId_ = 0;

    std::vector<Vec2> placeInZone(const Vec2& center, float radius, int count);
};
