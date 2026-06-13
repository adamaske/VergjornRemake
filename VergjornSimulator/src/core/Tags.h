#pragma once
#include <string>
#include <cstdint>

enum class EResource : uint8_t {
    Wood, BogIron, Charcoal, IronIngot,
    ToolAxe, WeaponSword, WeaponShield,
    Fish, Grain,
    COUNT
};

enum class EOccupation : uint8_t {
    Idle, Lumberjack, BogWorker, Smelter,
    Blacksmith, Farmer, Shipwright, Viking, Hauler, Builder,
    COUNT
};

enum class ESocialClass : uint8_t { Karl, Thrall, Jarl };

enum class EJobType : uint8_t { Chop, Haul, Build, Craft, DigBog, Farm, Fish };

enum class ESeason : uint8_t { Spring, Summer, Autumn, Winter };

// ── String helpers ────────────────────────────────────────────────────────────

inline const char* toString(EResource r) {
    switch (r) {
    case EResource::Wood:         return "Wood";
    case EResource::BogIron:      return "BogIron";
    case EResource::Charcoal:     return "Charcoal";
    case EResource::IronIngot:    return "IronIngot";
    case EResource::ToolAxe:      return "ToolAxe";
    case EResource::WeaponSword:  return "WeaponSword";
    case EResource::WeaponShield: return "WeaponShield";
    case EResource::Fish:         return "Fish";
    case EResource::Grain:        return "Grain";
    default:                      return "Unknown";
    }
}

inline const char* toString(EOccupation o) {
    switch (o) {
    case EOccupation::Idle:        return "Idle";
    case EOccupation::Lumberjack:  return "Lumberjack";
    case EOccupation::BogWorker:   return "BogWorker";
    case EOccupation::Smelter:     return "Smelter";
    case EOccupation::Blacksmith:  return "Blacksmith";
    case EOccupation::Farmer:      return "Farmer";
    case EOccupation::Shipwright:  return "Shipwright";
    case EOccupation::Viking:      return "Viking";
    case EOccupation::Hauler:      return "Hauler";
    case EOccupation::Builder:     return "Builder";
    default:                       return "Unknown";
    }
}

inline const char* toString(ESeason s) {
    switch (s) {
    case ESeason::Spring: return "Spring";
    case ESeason::Summer: return "Summer";
    case ESeason::Autumn: return "Autumn";
    case ESeason::Winter: return "Winter";
    default:              return "Unknown";
    }
}

inline const char* toString(ESocialClass c) {
    switch (c) {
    case ESocialClass::Karl:   return "Karl";
    case ESocialClass::Thrall: return "Thrall";
    case ESocialClass::Jarl:   return "Jarl";
    default:                   return "Unknown";
    }
}
