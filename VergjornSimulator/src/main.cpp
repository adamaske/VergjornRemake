#include "sim/WorldSim.h"
#include "world/MapRenderer.h"
#include <iostream>
#include <filesystem>
#include <stdexcept>

int main(int argc, char* argv[]) {
    int   targetDays  = 365;
    int   villagers   = 10;
    int   trees       = 48;   // distributed across 4 forest patches
    float worldRadius = 2000.f;

    if (argc >= 2) targetDays  = std::stoi(argv[1]);
    if (argc >= 3) villagers   = std::stoi(argv[2]);
    if (argc >= 4) trees       = std::stoi(argv[3]);

    std::filesystem::create_directories("output");

    WorldSim sim("output/stats.csv");
    sim.time.setTimeScale(60.f);
    sim.initialize(villagers, trees, worldRadius);

    // Render ASCII map to console and save to file
    MapRenderer renderer;
    renderer.print(sim.worldMap(), std::cout);
    renderer.printToFile(sim.worldMap(), "output/map.txt");

    sim.run(targetDays, /*statIntervalDays=*/1);

    return 0;
}
