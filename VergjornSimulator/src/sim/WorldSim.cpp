#include "WorldSim.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>

static const std::string NAMES[] = {
    "Bjorn","Sigrid","Leif","Astrid","Erik","Freya","Gunnar","Helga",
    "Ivar","Ragnhild","Olaf","Thora","Ulf","Ingrid","Sven","Brynja",
    "Hakon","Gudrun","Ragnar","Solveig"
};
static constexpr int NUM_NAMES = static_cast<int>(sizeof(NAMES)/sizeof(NAMES[0]));

WorldSim::WorldSim(const std::string& outputPath)
    : stats_(std::make_unique<StatsWriter>(outputPath))
{
    // Wire up job completion callback
    jobBoard.onJobCompleted = [this](const JobRecord& job) {
        onJobCompleted(job);
    };

    // Wire up time callbacks
    time.onNewDay = [this](int day, int year) {
        onNewDay(day, year);
    };
}

void WorldSim::initialize(int numVillagers, int numTrees, float worldRadius) {
    std::uniform_real_distribution<float> posDist(-worldRadius, worldRadius);

    // Seed the economy with a small wood stockpile so construction can start
    economy.deposit(EResource::Wood, 20);

    // Logging hut at origin (already complete — no construction phase for MVP)
    Building hut;
    hut.id         = nextBuildingId_++;
    hut.name       = "Logging Hut";
    hut.pos        = {0.f, 0.f};
    hut.state      = EBuildingState::Complete;
    hut.workerType = EOccupation::Lumberjack;
    hut.maxWorkers = 4;
    hut.workRadius = worldRadius;
    buildings_.push_back(hut);

    // Spawn lumberjack villagers near the hut
    for (int i = 0; i < numVillagers; ++i) {
        Vec2 pos {
            std::uniform_real_distribution<float>(-200.f, 200.f)(rng_),
            std::uniform_real_distribution<float>(-200.f, 200.f)(rng_)
        };
        spawnVillager(EOccupation::Lumberjack, ESocialClass::Karl, pos);
    }

    // Scatter trees
    for (int i = 0; i < numTrees; ++i) {
        Vec2 pos { posDist(rng_), posDist(rng_) };
        auto& tree = spawnTree(pos);
        postChopJob(tree);
    }

    std::cout << "[WorldSim] Initialized: "
              << numVillagers << " villagers, "
              << numTrees     << " trees, "
              << buildings_.size() << " building(s)\n";
}

void WorldSim::run(int targetDays, int statIntervalDays) {
    const float fixedDt = 0.1f;   // 10 ticks per real second

    // How many real seconds = targetDays game days?
    double targetGameSeconds = targetDays * SimTime::SecondsPerDay;
    int    lastStatDay       = -1;

    std::cout << "[WorldSim] Running " << targetDays << " game days "
              << "(time scale " << time.timeScale() << "x)...\n";

    while (time.totalGameSeconds() < targetGameSeconds) {
        tick(fixedDt);

        int curDay = time.day();
        if (curDay - lastStatDay >= statIntervalDays) {
            recordStats();
            lastStatDay = curDay;
        }
    }

    recordStats();  // final snapshot
    std::cout << "[WorldSim] Done. Day " << time.day()
              << ", Year " << time.year()
              << ", Wood=" << economy.stored(EResource::Wood) << '\n';
}

void WorldSim::tick(float fixedDt) {
    time.advance(fixedDt);
    tickNeeds(fixedDt);
    tickMovement(fixedDt);
    tickJobs(fixedDt);
    tickTrees(fixedDt);
}

// ── Needs ─────────────────────────────────────────────────────────────────────

void WorldSim::tickNeeds(float dt) {
    constexpr float hungerDecay = 0.005f;
    constexpr float warmthDecay = 0.003f;
    constexpr float restDecay   = 0.004f;

    // Winter doubles warmth decay
    float winterMult = (time.season() == ESeason::Winter) ? 2.f : 1.f;

    for (auto& v : villagers_) {
        v.hunger = std::max(0.f, v.hunger - hungerDecay * dt);
        v.warmth = std::max(0.f, v.warmth - warmthDecay * winterMult * dt);
        v.rest   = std::max(0.f, v.rest   - restDecay   * dt);
        // Morale tracks average needs with some inertia
        v.morale += (v.avgNeeds() - v.morale) * 0.01f * dt;
        v.morale  = std::max(0.f, std::min(1.f, v.morale));
    }
}

// ── Movement ──────────────────────────────────────────────────────────────────

void WorldSim::tickMovement(float dt) {
    constexpr float speed = 150.f;

    for (auto& v : villagers_) {
        if (v.isIdle()) continue;
        if (v.isAtJobSite()) continue;

        Vec2  dir  = (v.jobLocation - v.pos).normalized();
        float step = speed * dt;
        float dist = v.pos.distTo(v.jobLocation);
        float move = std::min(step, dist);
        v.pos += dir * move;
    }
}

// ── Jobs ──────────────────────────────────────────────────────────────────────

void WorldSim::tickJobs(float dt) {
    constexpr float workRate    = 5.f;   // work units per second
    constexpr float arrivalRad  = 100.f;

    for (auto& v : villagers_) {
        // ── Idle: find a job ─────────────────────────────────────────────────
        if (v.isIdle()) {
            JobHandle h;
            if (jobBoard.findBestJob(v.occupation, v.pos, h)) {
                if (jobBoard.claimJob(h, v.id)) {
                    const JobRecord* rec = jobBoard.getJob(h);
                    v.jobHandle   = h;
                    v.jobType     = rec->type;
                    v.jobLocation = rec->location;
                    v.workProgress= 0.f;
                }
            }
            continue;
        }

        // ── Travelling: wait until arrived ───────────────────────────────────
        if (!v.isAtJobSite(arrivalRad)) continue;

        // ── Working ──────────────────────────────────────────────────────────
        if (jobBoard.advanceJob(v.jobHandle, workRate * dt)) {
            // Job completed; onJobCompleted callback fires inside advanceJob.
            v.jobHandle.invalidate();
            v.workProgress = 0.f;
        }
    }
}

// ── Trees ─────────────────────────────────────────────────────────────────────

void WorldSim::tickTrees(float dt) {
    for (auto& tree : trees_) {
        if (tree.isAlive) continue;
        if (tree.respawnTime <= 0.f) continue;

        tree.respawnCountdown -= dt * time.timeScale();
        if (tree.respawnCountdown <= 0.f) {
            tree.isAlive = true;
            postChopJob(tree);
        }
    }
}

// ── Job completion ────────────────────────────────────────────────────────────

void WorldSim::onJobCompleted(const JobRecord& job) {
    if (job.type == EJobType::Chop && job.targetEntity) {
        auto* tree = reinterpret_cast<TreeNode*>(job.targetEntity);
        onChopCompleted(*tree, job);
    }
}

void WorldSim::onChopCompleted(TreeNode& tree, const JobRecord&) {
    economy.deposit(EResource::Wood, tree.woodYield);
    tree.chopJobHandle.invalidate();

    if (tree.respawnTime > 0.f) {
        tree.isAlive           = false;
        tree.respawnCountdown  = tree.respawnTime;
    }
}

// ── Immigration ───────────────────────────────────────────────────────────────

void WorldSim::checkImmigration() {
    // Simple rule: if avg morale > 0.6 and wood > 30, attract one new Karl
    if (villagers_.empty()) return;
    float totalMorale = 0.f;
    for (auto& v : villagers_) totalMorale += v.morale;
    float avgMorale = totalMorale / static_cast<float>(villagers_.size());

    if (avgMorale > 0.6f && economy.stored(EResource::Wood) > 30) {
        std::uniform_real_distribution<float> d(-300.f, 300.f);
        spawnVillager(EOccupation::Lumberjack, ESocialClass::Karl, {d(rng_), d(rng_)});
        std::cout << "[Immigration] New Karl arrived. Population: " << villagers_.size() << '\n';
    }
}

// ── Spawn helpers ─────────────────────────────────────────────────────────────

void WorldSim::spawnVillager(EOccupation occ, ESocialClass cls, const Vec2& pos) {
    Villager v;
    v.id          = nextVillagerId_++;
    v.name        = NAMES[v.id % NUM_NAMES];
    v.occupation  = occ;
    v.socialClass = cls;
    v.pos         = pos;
    villagers_.push_back(v);
}

TreeNode& WorldSim::spawnTree(const Vec2& pos, float chopWork, int wood) {
    TreeNode t;
    t.id        = nextTreeId_++;
    t.pos       = pos;
    t.chopWork  = chopWork;
    t.woodYield = wood;
    trees_.push_back(t);
    return trees_.back();
}

void WorldSim::postChopJob(TreeNode& tree) {
    JobRecord job;
    job.type                = EJobType::Chop;
    job.requiredOccupation  = EOccupation::Lumberjack;
    job.location            = tree.pos;
    job.priority            = 5;
    job.workRequired        = tree.chopWork;
    job.targetEntity        = &tree;

    tree.chopJobHandle = jobBoard.postJob(job);
}

// ── Stats ─────────────────────────────────────────────────────────────────────

void WorldSim::recordStats() {
    stats_->write(collectStats());
}

FrameStats WorldSim::collectStats() const {
    FrameStats s{};
    s.day         = time.day();
    s.year        = time.year();
    s.season      = time.season();
    s.gameSeconds = time.totalGameSeconds();
    s.population  = static_cast<int>(villagers_.size());
    s.openJobs    = jobBoard.openJobCount();

    for (int i = 0; i < static_cast<int>(EResource::COUNT); ++i)
        s.resources[i] = economy.stored(static_cast<EResource>(i));

    float sumH = 0, sumW = 0, sumR = 0, sumM = 0;
    int working = 0;
    for (auto& v : villagers_) {
        sumH += v.hunger; sumW += v.warmth; sumR += v.rest; sumM += v.morale;
        if (!v.isIdle()) working++;
    }
    int n = static_cast<int>(villagers_.size());
    if (n > 0) {
        s.avgHunger = sumH / n;
        s.avgWarmth = sumW / n;
        s.avgRest   = sumR / n;
        s.avgMorale = sumM / n;
    }
    s.workingVillagers = working;
    s.idleVillagers    = n - working;
    return s;
}

// ── Day events ────────────────────────────────────────────────────────────────

void WorldSim::onNewDay(int day, int year) {
    checkImmigration();

    // Replenish needs a little at dawn (simplified food/rest recovery)
    for (auto& v : villagers_) {
        // Eat from the food supply if hungry
        if (v.hunger < 0.5f && economy.tryWithdraw(EResource::Grain, 1)) {
            v.hunger = std::min(1.f, v.hunger + 0.4f);
        }
        // Rest recovers at night
        v.rest = std::min(1.f, v.rest + 0.3f);
    }

    if (day % 30 == 0) {
        std::cout << "[Day " << std::setw(4) << day
                  << " Y" << year
                  << " " << toString(time.season()) << "] "
                  << "Pop=" << villagers_.size()
                  << "  Wood=" << economy.stored(EResource::Wood)
                  << "  Jobs=" << jobBoard.openJobCount()
                  << '\n';
    }
}
