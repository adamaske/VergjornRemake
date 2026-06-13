#pragma once
#include "MapGen.h"
#include <ostream>
#include <string>

class MapRenderer {
public:
    // Grid dimensions for the ASCII output
    int gridW = 60;
    int gridH = 26;

    void print(const GeneratedMap& map, std::ostream& out) const;
    void printToFile(const GeneratedMap& map, const std::string& path) const;
};
