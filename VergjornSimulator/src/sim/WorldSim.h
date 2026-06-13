#pragma once
#include "core/SimTime.h"
#include "core/Economy.h"
#include "core/JobBoard.h"
#include "Villager.h"
#include "TreeNode.h"
#include "Building.h"
#include "output/StatsWriter.h"
#include <vector>
#include <memory>
#include <random>
#include <cstdint>

// Master simulation. Call initialize() then run().
class WorldSim {
public:
    SimTime  time;
    Economy  economy;
    JobBoard jobBoard;

    explicit WorldSim(const std::string& outputPath);

    void initialize(int numVillagers = 10, int numTrees = 50,
                    float worldRadius = 2000.f);

    // Run for targetDays in-game days, writing stats every statIntervalDays
    void run(int targetDays, int statIntervalDays = 1);

    // Single tick: advances by fixedDt real seconds
    void tick(float fixedDt = 0.1f);

    // Accessors for inspection
    const std::vector<Villager>&  villagers()  const { return villagers_; }
    const std::vector<TreeNode>&  trees()      const { return trees_; }
    const std::vector<Building>&  buildings()  const { return buildings_; }

private:
    std::vector<Villager>  villagers_;
    std::vector<TreeNode>  trees_;
    std::vector<Building>  buildings_;

    std::unique_ptr<StatsWriter> stats_;

    uint32_t nextVillagerId_ = 1;
    uint32_t nextTreeId_     = 1;
    uint32_t nextBuildingId_ = 1;

    std::mt19937 rng_{42};

    // ── Tick phases ──────────────────────────────────────────────────────────
    void tickNeeds(float dt);
    void tickMovement(float dt);
    void tickJobs(float dt);
    void tickTrees(float dt);        // respawn countdown
    void checkImmigration();

    // ── Helpers ──────────────────────────────────────────────────────────────
    void spawnVillager(EOccupation occ, ESocialClass cls, const Vec2& pos);
    TreeNode& spawnTree(const Vec2& pos, float chopWork = 20.f, int wood = 5);
    void postChopJob(TreeNode& tree);

    // ── Job completion ────────────────────────────────────────────────────────
    void onJobCompleted(const JobRecord& job);
    void onChopCompleted(TreeNode& tree, const JobRecord& job);

    // ── Stats ─────────────────────────────────────────────────────────────────
    void recordStats();
    FrameStats collectStats() const;

    // ── Day/season events ─────────────────────────────────────────────────────
    void onNewDay(int day, int year);
};
