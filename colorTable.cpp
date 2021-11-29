/**
 * @file   colorTable.cpp
 * @brief  .
 * @author Ángel Lorente Rogel
 * @date   02/11/2021
 */

#include "colorTable.h"
#include <tgmath.h> 
#include <opencv2/imgproc.hpp>

void ColorTable::GenerateBGRColorTable(int gridSize){
    std::unordered_map<std::string, Color> table;
    int gridSize_2 = gridSize*gridSize;
    int gridSize_3 = gridSize*gridSize*gridSize;
    double binSize = 256.0/gridSize;
    int binSize_half = (int)(binSize/2);

    for(int i = 0; i < gridSize_3; i++){
        int cluster_r, cluster_g, cluster_b;
        cluster_r = (i%gridSize_2)%gridSize;
        cluster_g = floor((i%gridSize_2)/gridSize);
        cluster_b = floor(i/gridSize_2);
        int r = (int)(cluster_r*binSize) + binSize_half;
        int g = (int)(cluster_g*binSize) + binSize_half;
        int b = (int)(cluster_b*binSize) + binSize_half;
        table.insert({std::to_string(i), Color{b, g, r}});
    }

    this->table = table;
}

void ColorTable::GenerateHSVColorTable(int bins){
    std::unordered_map<std::string, Color> table;
    double binSize = 181.0/bins;
    int binSize_half = (int)(binSize/2);

    for(int i = 0; i < bins; i++){
        int hue = (int)(i*binSize) + binSize_half;

        cv::Mat3b ColorBGRMat; 
        cv::Mat3b ColorHsvMat(cv::Vec3b(hue, 255, 255));
        cv::cvtColor(ColorHsvMat, ColorBGRMat, cv::COLOR_HSV2BGR);

        int r = ColorBGRMat[0][0][2];
        int g = ColorBGRMat[0][0][1];
        int b = ColorBGRMat[0][0][0];
        table.insert({std::to_string(i), Color{b, g, r}});
    }

    this->table = table;
}

std::string ColorTable::LookUpColor(int b, int g, int r){

    int min_dist = 255*3;
    std::string color_name = "";
    int new_r = r;
    int new_g = g;
    int new_b = b;

    for(auto& c: this->table){
        int r2 = c.second.r;
        int g2 = c.second.g;
        int b2 = c.second.b;
        int dist = abs(r-r2) + abs(g-g2) + abs(b-b2);
        if(dist<min_dist){
            min_dist = dist;
            new_r = r2;
            new_g = g2;
            new_b = b2;
            color_name = c.first;
        }
    }

    return color_name;
}

void ColorTable::GetBGR(std::string color_name, int &b, int &g, int &r){
    Color c = this->table.at(color_name);
    r = c.r;
    g = c.g;
    b = c.b;
}