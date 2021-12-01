/**
 * @file   image_analyzer.h
 * @brief  .
 * @author Ángel Lorente Rogel
 * @date   21/11/2021
 */

#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "colorTable.h"
#include "clusterer.h"

/**
 * The ImgAnalyzer interface declares the operations that all concrete products must
 * implement.
 */
class ImgProcessor{
public:
    ~ImgProcessor(){delete _clusterer;}
    void ProcessImg(const cv::Mat &img);
    std::vector<cv::Vec3b> GetColorPalette();
    void SubstituteColor(cv::Vec3b color, cv::Vec3b newColor, cv::Mat &img);
    void HueShift(cv::Vec3b color, cv::Vec3b newColor, cv::Mat &img);
private:
    void Initialize();
    std::vector<int> ComputeHistogram(cv::InputArray in);
    int GetClusterId(const cv::Vec3b sample, const std::vector<cv::Vec3b> centers);
    int ComputeDistance(const cv::Vec3b point1, const cv::Vec3b point2);

    int _paletteSize;
    Clusterer * _clusterer;
    cv::Mat _labels;
    std::vector<cv::Vec3b> _centers;
};