#pragma once
#include "core/Tags.h"
#include "core/MathTypes.h"
#include "core/JobBoard.h"
#include <string>
#include <cstdint>

struct Villager {
    uint32_t    id          = 0;
    std::string name;
    ESocialClass socialClass = ESocialClass::Karl;
    EOccupation  occupation  = EOccupation::Idle;

    // World position
    Vec2 pos;

    // Needs [0..1], 1 = fully satisfied, decays toward 0
    float hunger = 1.f;
    float warmth = 1.f;
    float rest   = 1.f;
    float morale = 0.7f;

    // Current job
    JobHandle   jobHandle;          // invalid = idle
    EJobType    jobType     = EJobType::Chop;
    Vec2        jobLocation;
    float       workProgress = 0.f;

    bool isIdle()      const { return !jobHandle.isValid(); }
    bool isAtJobSite(float radius = 100.f) const {
        return pos.distSqTo(jobLocation) <= radius * radius;
    }

    // Morale is derived from average of needs
    float avgNeeds() const { return (hunger + warmth + rest) / 3.f; }
};
