#include "sim/WorldSim.h"
#include <iostream>
#include <filesystem>
#include <stdexcept>

int main(int argc, char* argv[]) {
    int  targetDays  = 365;  // one in-game year
    int  villagers   = 10;
    int  trees       = 60;
    float worldRadius= 2000.f;

    // Optional CLI args: <days> <villagers> <trees>
    if (argc >= 2) targetDays = std::stoi(argv[1]);
    if (argc >= 3) villagers  = std::stoi(argv[2]);
    if (argc >= 4) trees      = std::stoi(argv[3]);

    std::filesystem::create_directories("output");

    WorldSim sim("output/stats.csv");

    // Run faster than real time: 1 tick = 0.1s real, TimeScale = 60x
    // => 1 game day (600 game-sec) in 10 real seconds
    sim.time.setTimeScale(60.f);

    sim.initialize(villagers, trees, worldRadius);
    sim.run(targetDays, /*statIntervalDays=*/1);

    return 0;
}
