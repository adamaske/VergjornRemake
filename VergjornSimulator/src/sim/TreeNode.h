#pragma once
#include "core/MathTypes.h"
#include "core/JobBoard.h"
#include <cstdint>

// A harvestable tree. Holds a posted Job.Chop; when that job completes
// WorldSim's job-completion handler calls onChopped() which deposits wood.
struct TreeNode {
    uint32_t id     = 0;
    Vec2     pos;
    int      woodYield   = 5;
    float    chopWork    = 20.f;
    float    respawnTime = 120.f;   // game-seconds until regrowth (0 = permanent)

    JobHandle chopJobHandle;

    bool  isAlive    = true;
    float respawnCountdown = 0.f;

    bool hasOpenJob() const { return chopJobHandle.isValid(); }
};
