#pragma once
#include "core/MathTypes.h"
#include "core/JobBoard.h"
#include <cstdint>

// A bog-iron deposit. Works identically to TreeNode: posts a DigBog job,
// WorldSim's completion handler deposits EResource::BogIron.
struct OreDeposit {
    uint32_t  id          = 0;
    Vec2      pos;
    int       oreYield    = 3;
    float     mineWork    = 30.f;
    float     respawnTime = 90.f;   // game-seconds until the deposit refills (0 = permanent)

    JobHandle mineJobHandle;
    bool      isAlive          = true;
    float     respawnCountdown = 0.f;

    bool hasOpenJob() const { return mineJobHandle.isValid(); }
};
