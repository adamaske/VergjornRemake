#include "MapRenderer.h"
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iomanip>

static void renderTo(const GeneratedMap& map, std::ostream& out, int W, int H) {
    const float wr   = map.worldRadius;
    const float cellW = 2.f * wr / W;
    const float cellH = 2.f * wr / H;

    // Fill grid with open ground
    std::vector<std::vector<char>> grid(H, std::vector<char>(W, '.'));

    // World pos -> grid cell
    auto toCell = [&](const Vec2& p) -> std::pair<int,int> {
        int col = static_cast<int>((p.x + wr) / cellW);
        int row = static_cast<int>((wr - p.y) / cellH);
        col = std::max(0, std::min(W - 1, col));
        row = std::max(0, std::min(H - 1, row));
        return {col, row};
    };

    // 1. River — draw first so zones can overwrite the river line where they sit
    for (int col = 0; col < W; ++col) {
        float wx = (col + 0.5f) * cellW - wr;
        float wy = map.riverSlope * wx + map.riverOffset;
        int   row = static_cast<int>((wr - wy) / cellH);
        if (row >= 0 && row < H) grid[row][col] = '~';
    }

    // 2. Zones (painted in order; later zones overwrite earlier ones)
    for (auto& zone : map.zones) {
        char zc = zoneChar(zone.type);
        for (int r = 0; r < H; ++r) {
            for (int c = 0; c < W; ++c) {
                float wx = (c + 0.5f) * cellW - wr;
                float wy = wr - (r + 0.5f) * cellH;
                if (zone.contains({wx, wy})) grid[r][c] = zc;
            }
        }
    }

    // 3. Individual resource nodes (override zone fill)
    for (auto& p : map.treePositions) {
        auto [col, row] = toCell(p);
        grid[row][col] = 't';
    }
    for (auto& p : map.orePositions) {
        auto [col, row] = toCell(p);
        grid[row][col] = 'o';
    }
    for (auto& p : map.fishPositions) {
        auto [col, row] = toCell(p);
        grid[row][col] = 'f';
    }

    // 4. Village marker on top
    auto [vc, vr] = toCell({0.f, 0.f});
    grid[vr][vc] = '@';

    // Count zone types for the summary line
    int nForest = 0, nBog = 0, nFish = 0;
    for (auto& z : map.zones) {
        if (z.type == EZoneType::Forest)            ++nForest;
        else if (z.type == EZoneType::BogIron)      ++nBog;
        else if (z.type == EZoneType::FishingGround)++nFish;
    }

    // Header
    out << "\n  Vergjorn World  (seed:" << map.seed
        << "  " << static_cast<int>(2 * wr) << "x"
        << static_cast<int>(2 * wr) << " units)\n";
    out << "  +" << std::string(W, '-') << "+\n";
    for (int r = 0; r < H; ++r) {
        out << "  |";
        for (int c = 0; c < W; ++c) out << grid[r][c];
        out << "|\n";
    }
    out << "  +" << std::string(W, '-') << "+\n";
    out << "  Legend : [@]=Village  [F]=Forest  [B]=Bog  [~]=River  [G]=Farmland\n";
    out << "           [t]=tree     [o]=ore     [f]=fish\n";
    out << "  Zones  : " << nForest << " Forest | " << nBog << " Bog | " << nFish << " Fishing\n";
    out << "  Nodes  : " << map.treePositions.size() << " trees  "
        << map.orePositions.size() << " ore deposits  "
        << map.fishPositions.size() << " fishing spots\n\n";
}

void MapRenderer::print(const GeneratedMap& map, std::ostream& out) const {
    renderTo(map, out, gridW, gridH);
}

void MapRenderer::printToFile(const GeneratedMap& map, const std::string& path) const {
    std::ofstream f(path);
    if (f.is_open()) renderTo(map, f, gridW, gridH);
}
