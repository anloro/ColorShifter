/**
 * @file   image_analyzer.cpp
 * @brief  Functions to analyze the colours of an image.
 * @author Ángel Lorente Rogel
 * @date   21/11/2021
 */


#include "image_analyzer.h"
#include "utils.h"


void BGRImgAnalyzer::Initialize(){
    // Get these from a configuration file
    int paletteSize = 5;
    int gridSize = 3;

    _paletteSize = paletteSize;
    _gridSize = gridSize;
}

void BGRImgAnalyzer::ProcessImg(const cv::Mat &img){

    this->Initialize();
    cv::Mat * img_p;
    cv::Mat imgClone = img.clone();
    img_p = &imgClone;
    _img = img_p;
    _table.GenerateBGRColorTable(_gridSize);
    _clusters = FixedSizeGridClusterization(img);
}

std::vector<cv::Vec3b> BGRImgAnalyzer::GetColorPalette(){

    std::vector<int> hist = ComputeHistogram();
    std::vector<int> ind = get_sorted_indexes<int>(hist);
    std::vector<cv::Vec3b> palette;

    // Get the palette
    int b, g, r;
    for(int i = 0; i < _paletteSize; i++){
        std::string color = std::to_string(ind.at(i));
        _table.GetBGR(color, b, g, r);
        printf("b %i, g %i, r %i \n", b, g, r);
        palette.push_back(cv::Vec3b(b, g, r));
    }

    return palette;
}

/*  ---------------------------------------------------------
    Clusterization with fixed size grid & ordering using a histogram
    ---------------------------------------------------------*/
std::vector<int> BGRImgAnalyzer::ComputeHistogram()
{      
    std::vector<int> histogram(_gridSize*_gridSize*_gridSize, 0);

    for(auto const& n: _clusters)
    {
        histogram[n.second]++;
    }

    return histogram;
}

std::map<int, int> BGRImgAnalyzer::FixedSizeGridClusterization(const cv::Mat &img)
{
    std::vector<cv::Mat> channels;
    cv::split(img,channels);

    std::map<int, int> clusters;

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
            int key = GetPixelId(cv::Point(row, col), img.cols);
            clusters.insert(std::pair<int, int>(key, clusterId)); // (pixel, clusterId)
        }
    }

    std::vector<cv::Mat>().swap(channels);
    return clusters;
}

int BGRImgAnalyzer::GetClusterId(const int b, const int g, const int r)
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
    Color change methods
    ---------------------------------------------------------*/
void BGRImgAnalyzer::SubstituteColor(cv::Vec3b color, cv::Vec3b newColor, cv::Mat &img){        

    int r, g, b;
    r = color[2];
    g = color[1];
    b = color[0];

    int rTransform, gTransform, bTransform;
    rTransform = newColor[2] - r; 
    gTransform = newColor[1] - g; 
    bTransform = newColor[0] - b;

    int colorClusterId = GetClusterId(b, g, r);
    for(auto const& n: _clusters)
    {
        if(n.second == colorClusterId){
            cv::Point pixelCoordinates = GetPixelCoordinates(n.first, img.cols);
            int row = pixelCoordinates.x;
            int columm = pixelCoordinates.y;

            cv::Vec3b colorVec = img.at<cv::Vec3b>(row, columm);
            r = colorVec[2];
            g = colorVec[1];
            b = colorVec[0];

            uchar newR = std::max(std::min(r + rTransform, 255), 0); 
            uchar newG = std::max(std::min(g + gTransform, 255), 0);                
            uchar newB = std::max(std::min(b + bTransform, 255), 0);
            cv::Vec3b newColorVec = cv::Vec3b(newB, newG, newR);
            
            img.at<cv::Vec3b>(row, columm) = newColorVec;
        }
    }
}

void BGRImgAnalyzer::HueShift(cv::Vec3b color, cv::Vec3b newColor, cv::Mat &img){        

    int r, g, b;
    r = color[2];
    g = color[1];
    b = color[0];

    cv::Mat3b newColorHsvMat, ColorHsvMat; 
    cv::Mat3b newColorBGRMat(cv::Vec3b(newColor[2], newColor[1], newColor[0]));
    cv::cvtColor(newColorBGRMat, newColorHsvMat, cv::COLOR_BGR2HSV);
    cv::Mat3b ColorBGRMat(cv::Vec3b(color[2], color[1], color[0]));
    cv::cvtColor(ColorBGRMat, ColorHsvMat, cv::COLOR_BGR2HSV);

    int hueTransform = newColorHsvMat[0][0][0] - ColorHsvMat[0][0][0]; 

    cv::Mat imgHSV;
    cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);
    int colorClusterId = GetClusterId(b, g, r);
    for(auto const& n: _clusters)
    {
        if(n.second == colorClusterId){
            cv::Point pixelCoordinates = GetPixelCoordinates(n.first, img.cols);
            int row = pixelCoordinates.x;
            int columm = pixelCoordinates.y;

            cv::Vec3b colorVec = imgHSV.at<cv::Vec3b>(row, columm);
            int hue = colorVec[0];
            int sat = colorVec[1];
            int val = colorVec[2];

            uchar newHue = (hue + hueTransform)%180; 
            cv::Vec3b newColorVec = cv::Vec3b(newHue, sat, val);
            
            imgHSV.at<cv::Vec3b>(row, columm) = newColorVec;
        }
    }

    cv::cvtColor(imgHSV, img, cv::COLOR_HSV2BGR);
}

void HSVImgAnalyzer::Initialize(){
    // Get these from a configuration file
    int paletteSize = 5;
    int bins = 8;

    _paletteSize = paletteSize;
    _bins = bins;
}

void HSVImgAnalyzer::ProcessImg(const cv::Mat &img){

    this->Initialize();
    cv::Mat * img_p;
    cv::Mat imgClone = img.clone();
    img_p = &imgClone;
    _img = img_p;
    _table.GenerateHSVColorTable(_bins);
    _clusters = FixedSizeClusterization(img);
}

std::vector<cv::Vec3b> HSVImgAnalyzer::GetColorPalette(){

    std::vector<int> hist = ComputeHistogram();
    std::vector<int> ind = get_sorted_indexes<int>(hist);
    std::vector<cv::Vec3b> palette;

    // Get the palette
    int b, g, r;
    for(int i = 0; i < _paletteSize; i++){
        std::string color = std::to_string(ind.at(i));
        _table.GetBGR(color, b, g, r);
        printf("b %i, g %i, r %i \n", b, g, r);
        palette.push_back(cv::Vec3b(b, g, r));
    }

    return palette;
}

std::vector<int> HSVImgAnalyzer::ComputeHistogram()
{      
    std::vector<int> histogram(_bins, 0);

    for(auto const& n: _clusters)
    {
        histogram[n.second]++;
    }

    return histogram;
}

std::map<int, int> HSVImgAnalyzer::FixedSizeClusterization(const cv::Mat &img)
{
    cv::Mat imgHSV;
    cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);
    std::vector<cv::Mat> channels;
    cv::split(imgHSV,channels);

    std::map<int, int> clusters;

    // Clusterization using a grid of fixed size
    for(int row = 0; row < imgHSV.rows; row++)
    {
        for(int col = 0; col < imgHSV.cols; col++)
        {
            // OpenCV reads image in BGR format
            int hue = channels[0].at<uchar>(row, col);

            int clusterId = GetClusterId(hue);
            int key = GetPixelId(cv::Point(row, col), imgHSV.cols);
            clusters.insert(std::pair<int, int>(key, clusterId)); // (pixel, clusterId)
        }
    }

    std::vector<cv::Mat>().swap(channels);
    return clusters;
}

int HSVImgAnalyzer::GetClusterId(const int hue)
{
    int clusterId;
    double binSize = 181.0 / _bins; // Hue range is [0, 180]

    clusterId =  (int)(floor(hue/binSize)); // [0, gridSize^3 -1]

    return clusterId;
}

void HSVImgAnalyzer::SubstituteColor(cv::Vec3b color, cv::Vec3b newColor, cv::Mat &img){        

    int r, g, b;
    r = color[2];
    g = color[1];
    b = color[0];

    int rTransform, gTransform, bTransform;
    rTransform = newColor[2] - r; 
    gTransform = newColor[1] - g; 
    bTransform = newColor[0] - b;

    cv::Mat3b ColorHsvMat; 
    cv::Mat3b ColorBGRMat(cv::Vec3b(color[2], color[1], color[0]));
    cv::cvtColor(ColorBGRMat, ColorHsvMat, cv::COLOR_BGR2HSV);
    int colorClusterId = GetClusterId(ColorHsvMat[0][0][0]);
    for(auto const& n: _clusters)
    {
        if(n.second == colorClusterId){
            cv::Point pixelCoordinates = GetPixelCoordinates(n.first, img.cols);
            int row = pixelCoordinates.x;
            int columm = pixelCoordinates.y;

            cv::Vec3b colorVec = img.at<cv::Vec3b>(row, columm);
            r = colorVec[2];
            g = colorVec[1];
            b = colorVec[0];

            uchar newR = std::max(std::min(r + rTransform, 255), 0); 
            uchar newG = std::max(std::min(g + gTransform, 255), 0);                
            uchar newB = std::max(std::min(b + bTransform, 255), 0);
            cv::Vec3b newColorVec = cv::Vec3b(newB, newG, newR);
            
            img.at<cv::Vec3b>(row, columm) = newColorVec;
        }
    }
}

void HSVImgAnalyzer::HueShift(cv::Vec3b color, cv::Vec3b newColor, cv::Mat &img){        

    int r, g, b;
    r = color[2];
    g = color[1];
    b = color[0];

    cv::Mat3b newColorHsvMat, ColorHsvMat; 
    cv::Mat3b newColorBGRMat(cv::Vec3b(newColor[2], newColor[1], newColor[0]));
    cv::cvtColor(newColorBGRMat, newColorHsvMat, cv::COLOR_BGR2HSV);
    cv::Mat3b ColorBGRMat(cv::Vec3b(color[2], color[1], color[0]));
    cv::cvtColor(ColorBGRMat, ColorHsvMat, cv::COLOR_BGR2HSV);

    int hueTransform = newColorHsvMat[0][0][0] - ColorHsvMat[0][0][0]; 

    cv::Mat imgHSV;
    cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);
    int colorClusterId = GetClusterId(ColorHsvMat[0][0][0]);
    for(auto const& n: _clusters)
    {
        if(n.second == colorClusterId){
            cv::Point pixelCoordinates = GetPixelCoordinates(n.first, img.cols);
            int row = pixelCoordinates.x;
            int columm = pixelCoordinates.y;

            cv::Vec3b colorVec = imgHSV.at<cv::Vec3b>(row, columm);
            int hue = colorVec[0];
            int sat = colorVec[1];
            int val = colorVec[2];

            uchar newHue = (hue + hueTransform)%180; 
            cv::Vec3b newColorVec = cv::Vec3b(newHue, sat, val);
            
            imgHSV.at<cv::Vec3b>(row, columm) = newColorVec;
        }
    }

    cv::cvtColor(imgHSV, img, cv::COLOR_HSV2BGR);
}

/*  ---------------------------------------------------------
    Codification of pixel positions
    ---------------------------------------------------------*/
int ImgAnalyzer::GetPixelId(cv::Point pixelCoordinates, int maxColums){
    int row = pixelCoordinates.x;
    int col = pixelCoordinates.y;
    return row*maxColums + col;
}

cv::Point ImgAnalyzer::GetPixelCoordinates(int pixelId, int maxColums){
    int row = floor(pixelId/maxColums);
    int col = pixelId%maxColums;
    return cv::Point(row, col);
}

/*  ---------------------------------------------------------
    Factory
    ---------------------------------------------------------*/
ImgAnalyzer* ImgAnalyzerFactory::GetObject(){
    // Get this option from a configuration file
    int option = 2;

    switch (option)
    {
        case 1:
            product = new BGRImgAnalyzer;
            break;
        case 2:
            product = new HSVImgAnalyzer;
            break;
        default:
            product = nullptr;
            break;
    }

    return product;
}
