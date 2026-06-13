#pragma once
#include "core/MathTypes.h"
#include <cstdint>
#include <string>

enum class EZoneType : uint8_t {
    Village,
    Forest,
    BogIron,
    FishingGround,
    Farmland,
    COUNT
};

inline const char* toString(EZoneType z) {
    switch (z) {
    case EZoneType::Village:       return "Village";
    case EZoneType::Forest:        return "Forest";
    case EZoneType::BogIron:       return "Bog";
    case EZoneType::FishingGround: return "Fishing";
    case EZoneType::Farmland:      return "Farmland";
    default:                       return "Unknown";
    }
}

inline char zoneChar(EZoneType z) {
    switch (z) {
    case EZoneType::Village:       return 'V';
    case EZoneType::Forest:        return 'F';
    case EZoneType::BogIron:       return 'B';
    case EZoneType::FishingGround: return '~';
    case EZoneType::Farmland:      return 'G';
    default:                       return '?';
    }
}

struct MapZone {
    int       id       = 0;
    EZoneType type     = EZoneType::Village;
    Vec2      center;
    float     radius   = 200.f;
    int       capacity = 0;
    std::string name;

    bool contains(const Vec2& p) const {
        return center.distSqTo(p) <= radius * radius;
    }
};
