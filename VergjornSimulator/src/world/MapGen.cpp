#include "MapGen.h"
#include <cmath>

static constexpr float PI = 3.14159265358979323846f;

GeneratedMap MapGen::generate(const MapConfig& cfg) {
    rng_.seed(cfg.seed);
    nextZoneId_ = 0;

    GeneratedMap result;
    result.worldRadius = cfg.worldRadius;
    result.seed        = cfg.seed;

    const float wr = cfg.worldRadius;

    // ── River ─────────────────────────────────────────────────────────────────
    std::uniform_real_distribution<float> slopeDist(-0.12f, 0.12f);
    std::uniform_real_distribution<float> offsetDist(-wr * 0.25f, wr * 0.25f);
    result.riverSlope  = slopeDist(rng_);
    result.riverOffset = offsetDist(rng_);

    // ── Village zone ──────────────────────────────────────────────────────────
    {
        MapZone z;
        z.id     = nextZoneId_++;
        z.type   = EZoneType::Village;
        z.center = {0.f, 0.f};
        z.radius = wr * 0.10f;
        z.name   = "Village";
        result.zones.push_back(z);
    }

    // ── Forest patches ────────────────────────────────────────────────────────
    // Evenly spread angularly around the village with random distance and jitter.
    {
        float minDist     = wr * 0.30f;
        float maxDist     = wr * 0.72f;
        float zoneRadius  = wr * 0.11f;
        std::uniform_real_distribution<float> distDist(minDist, maxDist);
        std::uniform_real_distribution<float> angJit(-0.45f, 0.45f);
        std::uniform_real_distribution<float> radVar(-wr * 0.02f, wr * 0.04f);

        for (int i = 0; i < cfg.numForestPatches; ++i) {
            float angle = (2.f * PI * i) / cfg.numForestPatches + angJit(rng_);
            float dist  = distDist(rng_);
            Vec2  center{std::cos(angle) * dist, std::sin(angle) * dist};

            MapZone z;
            z.id       = nextZoneId_++;
            z.type     = EZoneType::Forest;
            z.center   = center;
            z.radius   = zoneRadius + radVar(rng_);
            z.capacity = cfg.treesPerForest;
            z.name     = std::string("Forest ") + std::to_string(i + 1);
            result.zones.push_back(z);

            auto nodes = placeInZone(center, z.radius, cfg.treesPerForest);
            result.treePositions.insert(result.treePositions.end(), nodes.begin(), nodes.end());
        }
    }

    // ── Bog iron patches ──────────────────────────────────────────────────────
    // Placed near the river (bogs form in low, wet ground).
    {
        float zoneRadius = wr * 0.08f;
        std::uniform_real_distribution<float> xDist(-wr * 0.55f, wr * 0.55f);
        std::uniform_real_distribution<float> yJit(-wr * 0.06f, wr * 0.12f);

        for (int i = 0; i < cfg.numBogPatches; ++i) {
            float x = xDist(rng_);
            float y = result.riverSlope * x + result.riverOffset + yJit(rng_);
            Vec2  center{x, y};

            MapZone z;
            z.id       = nextZoneId_++;
            z.type     = EZoneType::BogIron;
            z.center   = center;
            z.radius   = zoneRadius;
            z.capacity = cfg.oresPerBog;
            z.name     = std::string("Bog ") + std::to_string(i + 1);
            result.zones.push_back(z);

            auto nodes = placeInZone(center, z.radius, cfg.oresPerBog);
            result.orePositions.insert(result.orePositions.end(), nodes.begin(), nodes.end());
        }
    }

    // ── Fishing grounds ───────────────────────────────────────────────────────
    // Placed directly on the river at separated x positions.
    {
        float zoneRadius = wr * 0.07f;
        float xStep      = wr * 1.0f / (cfg.numFishingGrounds + 1);
        std::uniform_real_distribution<float> xJit(-wr * 0.05f, wr * 0.05f);

        for (int i = 0; i < cfg.numFishingGrounds; ++i) {
            float x = -wr * 0.5f + xStep * (i + 1) + xJit(rng_);
            float y = result.riverSlope * x + result.riverOffset;
            Vec2  center{x, y};

            MapZone z;
            z.id       = nextZoneId_++;
            z.type     = EZoneType::FishingGround;
            z.center   = center;
            z.radius   = zoneRadius;
            z.capacity = cfg.fishSpotsPerGround;
            z.name     = std::string("Fishing ") + std::to_string(i + 1);
            result.zones.push_back(z);

            auto nodes = placeInZone(center, z.radius, cfg.fishSpotsPerGround);
            result.fishPositions.insert(result.fishPositions.end(), nodes.begin(), nodes.end());
        }
    }

    return result;
}

std::vector<Vec2> MapGen::placeInZone(const Vec2& center, float radius, int count) {
    std::vector<Vec2> out;
    out.reserve(count);
    std::uniform_real_distribution<float> angleDist(0.f, 2.f * PI);
    std::uniform_real_distribution<float> unitDist(0.f, 1.f);
    for (int i = 0; i < count; ++i) {
        float angle = angleDist(rng_);
        float r     = radius * std::sqrt(unitDist(rng_));   // uniform in disk
        out.push_back({center.x + std::cos(angle) * r,
                       center.y + std::sin(angle) * r});
    }
    return out;
}
