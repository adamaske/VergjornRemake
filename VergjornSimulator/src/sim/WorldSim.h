#pragma once
#include "core/SimTime.h"
#include "core/Economy.h"
#include "core/JobBoard.h"
#include "world/MapGen.h"
#include "Villager.h"
#include "TreeNode.h"
#include "OreDeposit.h"
#include "FishingSpot.h"
#include "Building.h"
#include "output/StatsWriter.h"
#include <vector>
#include <memory>
#include <random>
#include <cstdint>

class WorldSim {
public:
    SimTime  time;
    Economy  economy;
    JobBoard jobBoard;

    explicit WorldSim(const std::string& outputPath);

    void initialize(int numVillagers = 10, int numTrees = 48,
                    float worldRadius = 2000.f);

    void run(int targetDays, int statIntervalDays = 1);
    void tick(float fixedDt = 0.1f);

    // Read-only accessors
    const std::vector<Villager>&    villagers()    const { return villagers_; }
    const std::vector<TreeNode>&    trees()        const { return trees_; }
    const std::vector<OreDeposit>&  oreDeposits()  const { return oreDeposits_; }
    const std::vector<FishingSpot>& fishingSpots() const { return fishingSpots_; }
    const std::vector<Building>&    buildings()    const { return buildings_; }
    const GeneratedMap&             worldMap()     const { return map_; }

private:
    std::vector<Villager>    villagers_;
    std::vector<TreeNode>    trees_;
    std::vector<OreDeposit>  oreDeposits_;
    std::vector<FishingSpot> fishingSpots_;
    std::vector<Building>    buildings_;
    GeneratedMap             map_;

    std::unique_ptr<StatsWriter> stats_;

    uint32_t nextVillagerId_ = 1;
    uint32_t nextTreeId_     = 1;
    uint32_t nextOreId_      = 1;
    uint32_t nextFishId_     = 1;
    uint32_t nextBuildingId_ = 1;

    std::mt19937 rng_{42};

    // ── Tick phases ──────────────────────────────────────────────────────────
    void tickNeeds(float dt);
    void tickMovement(float dt);
    void tickJobs(float dt);
    void tickTrees(float dt);
    void tickOre(float dt);
    void tickFishing(float dt);
    void checkImmigration();

    // ── Spawn helpers ────────────────────────────────────────────────────────
    void         spawnVillager(EOccupation occ, ESocialClass cls, const Vec2& pos);
    TreeNode&    spawnTree(const Vec2& pos, float chopWork = 20.f, int wood = 5);
    OreDeposit&  spawnOreDeposit(const Vec2& pos, float mineWork = 30.f, int ore = 3);
    FishingSpot& spawnFishingSpot(const Vec2& pos, float fishWork = 15.f, int fish = 4);

    void postChopJob(TreeNode& tree);
    void postMineJob(OreDeposit& ore);
    void postFishJob(FishingSpot& spot);

    // ── Job completion ───────────────────────────────────────────────────────
    void onJobCompleted(const JobRecord& job);
    void onChopCompleted(TreeNode& tree);
    void onMineCompleted(OreDeposit& ore);
    void onFishCompleted(FishingSpot& spot);

    // ── Stats ────────────────────────────────────────────────────────────────
    void       recordStats();
    FrameStats collectStats() const;

    // ── Day events ───────────────────────────────────────────────────────────
    void onNewDay(int day, int year);
};
