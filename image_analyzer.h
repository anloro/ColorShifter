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
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "colorTable.h"

/**
 * The ImgAnalyzer interface declares the operations that all concrete products must
 * implement.
 */
class ImgAnalyzer {
 public:
  virtual std::vector<cv::Vec3b> GetColorPalette(const cv::Mat &img) = 0;
};

class BGRImgAnalyzer : public ImgAnalyzer {
public:
    std::vector<cv::Vec3b> GetColorPalette(const cv::Mat &img) override;
private:
    std::vector<int> ComputeHistogram(const std::map<cv::Point, int> clusters);
    std::map<cv::Point, int> FixedSizeGridClusterization(const cv::Mat &img, const int gridSize);

    ColorTable _table; // Depicts the centers of the clusters in BGR format
    std::map<cv::Point, int> _clusters;
};

// class HSVImgAnalyzer : public ImgAnalyzer {
// public:

// private:
//     std::vector<int> ComputeHistogram(const cv::Mat &img, const int gridSize);
// };

class ImgAnalyzerFactory {
public:
    ~ImgAnalyzerFactory(){delete product;}
    ImgAnalyzer* GetAnalyzer();

private:
    ImgAnalyzer* product;
};