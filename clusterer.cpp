/**
 * @file   image_analyzer.cpp
 * @brief  Functions to analyze the colours of an image.
 * @author Ángel Lorente Rogel
 * @date   21/11/2021
 */


#include "clusterer.h"
#include "utils.h"
#include <opencv2/imgproc.hpp>

/*  ---------------------------------------------------------
    BGR clusterization
    ---------------------------------------------------------*/
BGRClusterer::BGRClusterer(){
    this->Initialize();
}

void BGRClusterer::ComputeClusters(const cv::Mat &img, cv::InputOutputArray &labels, std::vector<cv::Vec3b> &centers){
    FixedSizeGridClusterization(img);
    ComputeCenters();
    _labels.copyTo(labels);
    centers = _centers;
}

void BGRClusterer::Initialize(){
    // Get these from a configuration file
    int gridSize = 3;

    _gridSize = gridSize;
}

void BGRClusterer::ComputeCenters(){
    std::vector<cv::Vec3b> centers;

    int gridSize_2 = _gridSize*_gridSize;
    int gridSize_3 = _gridSize*_gridSize*_gridSize;
    double binSize = 256.0/_gridSize;
    int binSize_half = (int)(binSize/2);

    for(int i = 0; i < gridSize_3; i++){
        int cluster_r, cluster_g, cluster_b;
        cluster_r = (i%gridSize_2)%_gridSize;
        cluster_g = floor((i%gridSize_2)/_gridSize);
        cluster_b = floor(i/gridSize_2);
        int r = (int)(cluster_r*binSize) + binSize_half;
        int g = (int)(cluster_g*binSize) + binSize_half;
        int b = (int)(cluster_b*binSize) + binSize_half;
        centers.push_back(cv::Vec3b(b, g, r));
    }

    _centers = centers;
}

void BGRClusterer::FixedSizeGridClusterization(const cv::Mat &img)
{
    std::vector<cv::Mat> channels;
    cv::split(img,channels);

    cv::Mat labels = cv::Mat(img.size(), CV_32FC1);

    // Clusterization using a grid of fixed size
    for(int row = 0; row < img.rows; row++)
    {
        for(int col = 0; col < img.cols; col++)
        {
            // OpenCV reads image in BGR format
            int r = channels[2].at<uchar>(row, col);
            int g = channels[1].at<uchar>(row, col);
            int b = channels[0].at<uchar>(row, col);

            int clusterId =  GetClusterId(b, g, r);
            labels.at<int>(row, col) = clusterId;
        }
    }

    std::vector<cv::Mat>().swap(channels);
    labels.copyTo(_labels);
}

int BGRClusterer::GetClusterId(const int b, const int g, const int r)
{
    int clusterId;
    double binSize = 256.0 / _gridSize; // BGR range is [0, 255]

    int clusterR = (int)(floor(r/binSize)); // bounded between [0, gridSize-1]
    int clusterG = (int)(floor(g/binSize));
    int clusterB = (int)(floor(b/binSize));

    clusterId =  clusterR + clusterG*_gridSize + clusterB*_gridSize*_gridSize; // [0, gridSize^3 -1]

    return clusterId;
}

/*  ---------------------------------------------------------
    Hue Clusterer member functions
    ---------------------------------------------------------*/
HueClusterer::HueClusterer(){
    this->Initialize();
}

void HueClusterer::ComputeClusters(const cv::Mat &img, cv::InputOutputArray &labels, std::vector<cv::Vec3b> &centers){
    FixedSizeGridClusterization(img);
    ComputeCenters();
    _labels.copyTo(labels);
    centers = _centers;
}

void HueClusterer::Initialize(){
    // Get these from a configuration file
    int gridSize = 8;

    _gridSize = gridSize;
}

void HueClusterer::ComputeCenters(){
    std::vector<cv::Vec3b> centers;

    double binSize = 181.0/_gridSize;
    int binSize_half = (int)(binSize/2);

    for(int i = 0; i < _gridSize; i++){
        int hue = (int)(i*binSize) + binSize_half;

        cv::Mat3b ColorBGRMat; 
        cv::Mat3b ColorHsvMat(cv::Vec3b(hue, 255, 255));
        cv::cvtColor(ColorHsvMat, ColorBGRMat, cv::COLOR_HSV2BGR);

        int r = ColorBGRMat[0][0][2];
        int g = ColorBGRMat[0][0][1];
        int b = ColorBGRMat[0][0][0];
        centers.push_back(cv::Vec3b(b, g, r));
    }

    _centers = centers;
}

void HueClusterer::FixedSizeGridClusterization(const cv::Mat &img)
{
    cv::Mat imgHSV;
    cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);
    std::vector<cv::Mat> channels;
    cv::split(imgHSV,channels);

    cv::Mat labels = cv::Mat(img.size(), CV_32FC1);

    // Clusterization using a grid of fixed size
    for(int row = 0; row < imgHSV.rows; row++)
    {
        for(int col = 0; col < imgHSV.cols; col++)
        {
            int hue = channels[0].at<uchar>(row, col);

            int clusterId = GetClusterId(hue);
            labels.at<int>(row, col) = clusterId;
        }
    }

    std::vector<cv::Mat>().swap(channels);
    _labels = labels;
}

int HueClusterer::GetClusterId(const int hue)
{
    int clusterId;
    double binSize = 181.0 / _gridSize; // Hue range is [0, 180]

    clusterId =  (int)(floor(hue/binSize)); // [0, gridSize^3 -1]

    return clusterId;
}

/*  ---------------------------------------------------------
    Codification of pixel positions
    ---------------------------------------------------------*/
int Clusterer::GetPixelId(cv::Point pixelCoordinates, int maxColums){
    int row = pixelCoordinates.x;
    int col = pixelCoordinates.y;
    return row*maxColums + col;
}

cv::Point Clusterer::GetPixelCoordinates(int pixelId, int maxColums){
    int row = floor(pixelId/maxColums);
    int col = pixelId%maxColums;
    return cv::Point(row, col);
}

/*  ---------------------------------------------------------
    Factory
    ---------------------------------------------------------*/
Clusterer* ClustererFactory::GetObject(){
    // Get this option from a configuration file
    int option = 2;

    switch (option)
    {
        case 1:
            product = new BGRClusterer;
            break;
        case 2:
            product = new HueClusterer;
            break;
        default:
            product = nullptr;
            break;
    }

    return product;
}
