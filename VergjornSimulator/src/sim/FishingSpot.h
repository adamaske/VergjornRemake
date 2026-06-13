#pragma once
#include "core/MathTypes.h"
#include "core/JobBoard.h"
#include <cstdint>

// A harvestable fishing spot on the river. Posts a Fish job;
// WorldSim's completion handler deposits EResource::Fish.
struct FishingSpot {
    uint32_t  id          = 0;
    Vec2      pos;
    int       fishYield   = 4;
    float     fishWork    = 15.f;
    float     respawnTime = 45.f;

    JobHandle fishJobHandle;
    bool      isAlive          = true;
    float     respawnCountdown = 0.f;

    bool hasOpenJob() const { return fishJobHandle.isValid(); }
};
