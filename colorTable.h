/**
 * @file   colorTable.h
 * @brief  .
 * @author Ángel Lorente Rogel
 * @date   02/11/2021
 */

#pragma once

#include <string>
#include <array>
#include <unordered_map>

// constexpr int TableSize = 139; 

struct Color{
    int b;
    int g; 
    int r; 
};

class ColorTable{

public:
    // CONSTRUCTOR
    ColorTable() = default;

    // MEMBER FUNCTIONS
    void GenerateColorTable(int gridSize);
    std::string LookUpColor(int r, int g, int b);
    void GetRGB(std::string color_name, int &r, int &g, int &b);

private:
    // This is a color table with the format
    std::unordered_map<std::string, Color> table;
};