#pragma once
#include "core/Tags.h"
#include "core/MathTypes.h"
#include <string>
#include <cstdint>

enum class EBuildingState : uint8_t { Planned, Hauling, Building, Complete };

struct Building {
    uint32_t    id          = 0;
    std::string name;
    Vec2        pos;
    EBuildingState state    = EBuildingState::Complete;
    EOccupation workerType  = EOccupation::Idle;
    int         maxWorkers  = 2;
    float       workRadius  = 3000.f;
};
