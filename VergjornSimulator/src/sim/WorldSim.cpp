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
    jobBoard.onJobCompleted = [this](const JobRecord& job) { onJobCompleted(job); };
    time.onNewDay           = [this](int day, int year)    { onNewDay(day, year); };
}

// ── Initialize ────────────────────────────────────────────────────────────────

void WorldSim::initialize(int numVillagers, int numTrees, float worldRadius) {
    // ── Generate map ─────────────────────────────────────────────────────────
    MapConfig cfg;
    cfg.worldRadius        = worldRadius;
    cfg.numForestPatches   = 4;
    cfg.treesPerForest     = std::max(1, numTrees / cfg.numForestPatches);
    cfg.numBogPatches      = 2;
    cfg.oresPerBog         = 6;
    cfg.numFishingGrounds  = 2;
    cfg.fishSpotsPerGround = 5;
    cfg.seed               = 42;

    MapGen gen;
    map_ = gen.generate(cfg);

    // ── Seed economy ─────────────────────────────────────────────────────────
    economy.deposit(EResource::Wood, 20);

    // ── Place buildings (one per zone type, at zone center) ──────────────────
    bool placedLogHut = false, placedBogWorks = false, placedFishHut = false;
    for (auto& zone : map_.zones) {
        Building bld;
        bld.id    = nextBuildingId_++;
        bld.pos   = zone.center;
        bld.state = EBuildingState::Complete;

        if (zone.type == EZoneType::Forest && !placedLogHut) {
            bld.name       = "Logging Hut";
            bld.workerType = EOccupation::Lumberjack;
            bld.maxWorkers = 6;
            bld.workRadius = worldRadius;
            placedLogHut   = true;
        } else if (zone.type == EZoneType::BogIron && !placedBogWorks) {
            bld.name       = "Bog Works";
            bld.workerType = EOccupation::BogWorker;
            bld.maxWorkers = 4;
            bld.workRadius = worldRadius;
            placedBogWorks = true;
        } else if (zone.type == EZoneType::FishingGround && !placedFishHut) {
            bld.name       = "Fishing Hut";
            bld.workerType = EOccupation::Fisher;
            bld.maxWorkers = 4;
            bld.workRadius = worldRadius;
            placedFishHut  = true;
        } else {
            continue;
        }
        buildings_.push_back(bld);
    }

    // ── Populate resource nodes (reserve first — we store raw pointers) ───────
    trees_.reserve(map_.treePositions.size());
    for (auto& p : map_.treePositions) {
        auto& tree = spawnTree(p);
        postChopJob(tree);
    }

    oreDeposits_.reserve(map_.orePositions.size());
    for (auto& p : map_.orePositions) {
        auto& ore = spawnOreDeposit(p);
        postMineJob(ore);
    }

    fishingSpots_.reserve(map_.fishPositions.size());
    for (auto& p : map_.fishPositions) {
        auto& spot = spawnFishingSpot(p);
        postFishJob(spot);
    }

    // ── Spawn villagers distributed by available resource zones ───────────────
    int totalNodes = static_cast<int>(
        map_.treePositions.size() + map_.orePositions.size() + map_.fishPositions.size());
    if (totalNodes == 0) totalNodes = 1;

    int nLumber = numVillagers * static_cast<int>(map_.treePositions.size()) / totalNodes;
    int nBog    = numVillagers * static_cast<int>(map_.orePositions.size())   / totalNodes;
    int nFish   = numVillagers - nLumber - nBog;

    std::uniform_real_distribution<float> near(-200.f, 200.f);
    auto nearVillage = [&]() -> Vec2 { return {near(rng_), near(rng_)}; };

    for (int i = 0; i < nLumber; ++i) spawnVillager(EOccupation::Lumberjack, ESocialClass::Karl, nearVillage());
    for (int i = 0; i < nBog;    ++i) spawnVillager(EOccupation::BogWorker,  ESocialClass::Karl, nearVillage());
    for (int i = 0; i < nFish;   ++i) spawnVillager(EOccupation::Fisher,     ESocialClass::Karl, nearVillage());

    std::cout << "[WorldSim] Initialized: "
              << numVillagers << " villagers ("
              << nLumber << " lumberjacks, "
              << nBog    << " bog workers, "
              << nFish   << " fishers), "
              << trees_.size()       << " trees, "
              << oreDeposits_.size() << " ore deposits, "
              << fishingSpots_.size()<< " fishing spots, "
              << buildings_.size()   << " buildings\n";
}

// ── Run loop ──────────────────────────────────────────────────────────────────

void WorldSim::run(int targetDays, int statIntervalDays) {
    const float fixedDt         = 0.1f;
    double      targetGameSecs  = targetDays * SimTime::SecondsPerDay;
    int         lastStatDay     = -1;

    std::cout << "[WorldSim] Running " << targetDays << " game days "
              << "(time scale " << time.timeScale() << "x)...\n";

    while (time.totalGameSeconds() < targetGameSecs) {
        tick(fixedDt);

        int curDay = time.day();
        if (curDay - lastStatDay >= statIntervalDays) {
            recordStats();
            lastStatDay = curDay;
        }
    }

    recordStats();
    std::cout << "[WorldSim] Done. Day " << time.day()
              << ", Year " << time.year()
              << "  Wood=" << economy.stored(EResource::Wood)
              << "  BogIron=" << economy.stored(EResource::BogIron)
              << "  Fish=" << economy.stored(EResource::Fish)
              << '\n';
}

void WorldSim::tick(float fixedDt) {
    time.advance(fixedDt);
    tickNeeds(fixedDt);
    tickMovement(fixedDt);
    tickJobs(fixedDt);
    tickTrees(fixedDt);
    tickOre(fixedDt);
    tickFishing(fixedDt);
}

// ── Needs ─────────────────────────────────────────────────────────────────────

void WorldSim::tickNeeds(float dt) {
    constexpr float hungerDecay = 0.005f;
    constexpr float warmthDecay = 0.003f;
    constexpr float restDecay   = 0.004f;

    float winterMult = (time.season() == ESeason::Winter) ? 2.f : 1.f;

    for (auto& v : villagers_) {
        v.hunger = std::max(0.f, v.hunger - hungerDecay * dt);
        v.warmth = std::max(0.f, v.warmth - warmthDecay * winterMult * dt);
        v.rest   = std::max(0.f, v.rest   - restDecay   * dt);
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
        float dist = v.pos.distTo(v.jobLocation);
        float move = std::min(speed * dt, dist);
        v.pos += dir * move;
    }
}

// ── Jobs ──────────────────────────────────────────────────────────────────────

void WorldSim::tickJobs(float dt) {
    constexpr float workRate   = 5.f;
    constexpr float arrivalRad = 100.f;

    for (auto& v : villagers_) {
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

        if (!v.isAtJobSite(arrivalRad)) continue;

        if (jobBoard.advanceJob(v.jobHandle, workRate * dt)) {
            v.jobHandle.invalidate();
            v.workProgress = 0.f;
        }
    }
}

// ── Resource node respawns ────────────────────────────────────────────────────

void WorldSim::tickTrees(float dt) {
    for (auto& tree : trees_) {
        if (tree.isAlive || tree.respawnTime <= 0.f) continue;
        tree.respawnCountdown -= dt * time.timeScale();
        if (tree.respawnCountdown <= 0.f) {
            tree.isAlive = true;
            postChopJob(tree);
        }
    }
}

void WorldSim::tickOre(float dt) {
    for (auto& ore : oreDeposits_) {
        if (ore.isAlive || ore.respawnTime <= 0.f) continue;
        ore.respawnCountdown -= dt * time.timeScale();
        if (ore.respawnCountdown <= 0.f) {
            ore.isAlive = true;
            postMineJob(ore);
        }
    }
}

void WorldSim::tickFishing(float dt) {
    for (auto& spot : fishingSpots_) {
        if (spot.isAlive || spot.respawnTime <= 0.f) continue;
        spot.respawnCountdown -= dt * time.timeScale();
        if (spot.respawnCountdown <= 0.f) {
            spot.isAlive = true;
            postFishJob(spot);
        }
    }
}

// ── Job completion ────────────────────────────────────────────────────────────

void WorldSim::onJobCompleted(const JobRecord& job) {
    switch (job.type) {
    case EJobType::Chop:
        if (job.targetEntity) onChopCompleted(*reinterpret_cast<TreeNode*>(job.targetEntity));
        break;
    case EJobType::DigBog:
        if (job.targetEntity) onMineCompleted(*reinterpret_cast<OreDeposit*>(job.targetEntity));
        break;
    case EJobType::Fish:
        if (job.targetEntity) onFishCompleted(*reinterpret_cast<FishingSpot*>(job.targetEntity));
        break;
    default:
        break;
    }
}

void WorldSim::onChopCompleted(TreeNode& tree) {
    economy.deposit(EResource::Wood, tree.woodYield);
    tree.chopJobHandle.invalidate();
    if (tree.respawnTime > 0.f) {
        tree.isAlive          = false;
        tree.respawnCountdown = tree.respawnTime;
    }
}

void WorldSim::onMineCompleted(OreDeposit& ore) {
    economy.deposit(EResource::BogIron, ore.oreYield);
    ore.mineJobHandle.invalidate();
    if (ore.respawnTime > 0.f) {
        ore.isAlive          = false;
        ore.respawnCountdown = ore.respawnTime;
    }
}

void WorldSim::onFishCompleted(FishingSpot& spot) {
    economy.deposit(EResource::Fish, spot.fishYield);
    spot.fishJobHandle.invalidate();
    if (spot.respawnTime > 0.f) {
        spot.isAlive          = false;
        spot.respawnCountdown = spot.respawnTime;
    }
}

// ── Immigration ───────────────────────────────────────────────────────────────

void WorldSim::checkImmigration() {
    if (villagers_.empty()) return;

    float totalMorale = 0.f;
    for (auto& v : villagers_) totalMorale += v.morale;
    float avgMorale = totalMorale / static_cast<float>(villagers_.size());

    if (avgMorale < 0.6f || economy.stored(EResource::Wood) < 30) return;

    // Only attract immigrants when jobs genuinely go unfilled
    if (jobBoard.openJobCount() < 3) return;

    // 25% chance per eligible day — keeps growth gradual and stochastic
    std::uniform_real_distribution<float> chance(0.f, 1.f);
    if (chance(rng_) > 0.25f) return;

    // Pick the occupation most under-represented relative to available nodes
    int nLumber = 0, nBog = 0, nFish = 0;
    for (auto& v : villagers_) {
        if (v.occupation == EOccupation::Lumberjack) ++nLumber;
        else if (v.occupation == EOccupation::BogWorker) ++nBog;
        else if (v.occupation == EOccupation::Fisher)    ++nFish;
    }

    int totalNodes = static_cast<int>(
        trees_.size() + oreDeposits_.size() + fishingSpots_.size());
    if (totalNodes == 0) return;

    // Pick occupation most under-represented relative to available nodes
    float treeRatio = trees_.empty()       ? 0.f : (float)nLumber / trees_.size();
    float oreRatio  = oreDeposits_.empty() ? 0.f : (float)nBog    / oreDeposits_.size();
    float fishRatio = fishingSpots_.empty()? 0.f : (float)nFish   / fishingSpots_.size();

    EOccupation newOcc;
    if (treeRatio <= oreRatio && treeRatio <= fishRatio) newOcc = EOccupation::Lumberjack;
    else if (oreRatio <= fishRatio)                       newOcc = EOccupation::BogWorker;
    else                                                  newOcc = EOccupation::Fisher;

    std::uniform_real_distribution<float> d(-300.f, 300.f);
    spawnVillager(newOcc, ESocialClass::Karl, {d(rng_), d(rng_)});
    std::cout << "[Immigration] New " << toString(newOcc)
              << " arrived. Population: " << villagers_.size() << '\n';
}

// ── Spawn helpers ─────────────────────────────────────────────────────────────

void WorldSim::spawnVillager(EOccupation occ, ESocialClass cls, const Vec2& pos) {
    Villager v;
    v.id         = nextVillagerId_++;
    v.name       = NAMES[v.id % NUM_NAMES];
    v.occupation = occ;
    v.socialClass= cls;
    v.pos        = pos;
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

OreDeposit& WorldSim::spawnOreDeposit(const Vec2& pos, float mineWork, int ore) {
    OreDeposit o;
    o.id       = nextOreId_++;
    o.pos      = pos;
    o.mineWork = mineWork;
    o.oreYield = ore;
    oreDeposits_.push_back(o);
    return oreDeposits_.back();
}

FishingSpot& WorldSim::spawnFishingSpot(const Vec2& pos, float fishWork, int fish) {
    FishingSpot f;
    f.id       = nextFishId_++;
    f.pos      = pos;
    f.fishWork = fishWork;
    f.fishYield= fish;
    fishingSpots_.push_back(f);
    return fishingSpots_.back();
}

void WorldSim::postChopJob(TreeNode& tree) {
    JobRecord job;
    job.type               = EJobType::Chop;
    job.requiredOccupation = EOccupation::Lumberjack;
    job.location           = tree.pos;
    job.priority           = 5;
    job.workRequired       = tree.chopWork;
    job.targetEntity       = &tree;
    tree.chopJobHandle     = jobBoard.postJob(job);
}

void WorldSim::postMineJob(OreDeposit& ore) {
    JobRecord job;
    job.type               = EJobType::DigBog;
    job.requiredOccupation = EOccupation::BogWorker;
    job.location           = ore.pos;
    job.priority           = 5;
    job.workRequired       = ore.mineWork;
    job.targetEntity       = &ore;
    ore.mineJobHandle      = jobBoard.postJob(job);
}

void WorldSim::postFishJob(FishingSpot& spot) {
    JobRecord job;
    job.type               = EJobType::Fish;
    job.requiredOccupation = EOccupation::Fisher;
    job.location           = spot.pos;
    job.priority           = 5;
    job.workRequired       = spot.fishWork;
    job.targetEntity       = &spot;
    spot.fishJobHandle     = jobBoard.postJob(job);
}

// ── Stats ─────────────────────────────────────────────────────────────────────

void WorldSim::recordStats() { stats_->write(collectStats()); }

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
    int   working = 0;
    for (auto& v : villagers_) {
        sumH += v.hunger; sumW += v.warmth;
        sumR += v.rest;   sumM += v.morale;
        if (!v.isIdle()) ++working;
    }
    int n = static_cast<int>(villagers_.size());
    if (n > 0) {
        s.avgHunger = sumH / n; s.avgWarmth = sumW / n;
        s.avgRest   = sumR / n; s.avgMorale = sumM / n;
    }
    s.workingVillagers = working;
    s.idleVillagers    = n - working;
    return s;
}

// ── Day events ────────────────────────────────────────────────────────────────

void WorldSim::onNewDay(int day, int year) {
    checkImmigration();

    for (auto& v : villagers_) {
        // Eat fish first, then grain
        if (v.hunger < 0.5f) {
            if (economy.tryWithdraw(EResource::Fish, 1) ||
                economy.tryWithdraw(EResource::Grain, 1)) {
                v.hunger = std::min(1.f, v.hunger + 0.4f);
            }
        }
        v.rest = std::min(1.f, v.rest + 0.3f);
    }

    if (day % 30 == 0) {
        std::cout << "[Day " << std::setw(4) << day
                  << " Y" << year
                  << " " << toString(time.season()) << "] "
                  << "Pop="     << villagers_.size()
                  << "  Wood="  << economy.stored(EResource::Wood)
                  << "  Ore="   << economy.stored(EResource::BogIron)
                  << "  Fish="  << economy.stored(EResource::Fish)
                  << "  Jobs="  << jobBoard.openJobCount()
                  << '\n';
    }
}
