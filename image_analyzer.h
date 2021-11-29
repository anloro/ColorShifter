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

/**
 * The ImgAnalyzer interface declares the operations that all concrete products must
 * implement.
 */
class ImgAnalyzer {
public:
    virtual void ProcessImg(const cv::Mat &img) = 0;
    virtual std::vector<cv::Vec3b> GetColorPalette() = 0;
    virtual void SubstituteColor(cv::Vec3b color, cv::Vec3b newColor, cv::Mat &img) = 0;
    virtual void HueShift(cv::Vec3b color, cv::Vec3b newColor, cv::Mat &img) = 0;
protected:
    int GetPixelId(cv::Point pixelCoordinates, int maxColums);
    cv::Point GetPixelCoordinates(int pixelId, int maxColums);
};

class BGRImgAnalyzer : public ImgAnalyzer {
public:
    ~BGRImgAnalyzer(){delete _img;}
    void ProcessImg(const cv::Mat &img) override;
    std::vector<cv::Vec3b> GetColorPalette() override;
    void SubstituteColor(cv::Vec3b color, cv::Vec3b newColor, cv::Mat &img) override;
    void HueShift(cv::Vec3b color, cv::Vec3b newColor, cv::Mat &img) override;
private:
    void Initialize();
    std::vector<int> ComputeHistogram();
    std::map<int, int> FixedSizeGridClusterization(const cv::Mat &img);
    int GetClusterId(const int b, const int g, const int r);

    cv::Mat* _img;
    ColorTable _table; // Depicts the centers of the clusters in BGR format
    std::map<int, int> _clusters; // [pixelId, clusterId]
    int _gridSize;
    int _paletteSize;
};

class HSVImgAnalyzer : public ImgAnalyzer {
public:
    ~HSVImgAnalyzer(){delete _img;}
    void ProcessImg(const cv::Mat &img) override;
    std::vector<cv::Vec3b> GetColorPalette() override;
    void SubstituteColor(cv::Vec3b color, cv::Vec3b newColor, cv::Mat &img) override;
    void HueShift(cv::Vec3b color, cv::Vec3b newColor, cv::Mat &img) override;
private:
    void Initialize();
    std::vector<int> ComputeHistogram();
    std::map<int, int> FixedSizeClusterization(const cv::Mat &img);
    int GetClusterId(const int hue);

    cv::Mat* _img;
    ColorTable _table; // Depicts the centers of the clusters in Hue
    std::map<int, int> _clusters; // [pixelId, clusterId]
    int _bins;
    int _paletteSize;
};

class ImgAnalyzerFactory {
public:
    ~ImgAnalyzerFactory(){delete product;}
    ImgAnalyzer* GetObject();

private:
    ImgAnalyzer* product;
};