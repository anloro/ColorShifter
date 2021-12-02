/**
 * @file   image_analyzer.cpp
 * @brief  Functions to analyze the colours of an image.
 * @author Ángel Lorente Rogel
 * @date   21/11/2021
 */


#include "image_processor.h"
#include "utils.h"

#define PALETTE_SIZE 5;

void ImgProcessor::Initialize(){
    // Get these from a configuration file
    int paletteSize = PALETTE_SIZE;

    ClustererFactory clustererCreator = ClustererFactory();
    _clusterer = clustererCreator.GetObject();

    _paletteSize = paletteSize;
}

void ImgProcessor::ProcessImg(const cv::Mat &img){

    this->Initialize();
    _clusterer->ComputeClusters(img, _labels, _centers);
}

std::vector<cv::Vec3b> ImgProcessor::GetColorPalette(){

    std::vector<int> hist = ComputeHistogram(_labels);
    std::vector<int> ind = get_sorted_indexes<int>(hist);
    std::vector<cv::Vec3b> palette;

    // Get the palette
    for(int i = 0; i < _paletteSize; i++){
        cv::Vec3b color = _centers[ind[i]];
        int b = color[0];
        int g = color[1];
        int r = color[2];
        printf("b %i, g %i, r %i \n", b, g, r);
        palette.push_back(cv::Vec3b(b, g, r));
    }

    return palette;
}

/*  ---------------------------------------------------------
    Compute histogram
    ---------------------------------------------------------*/
std::vector<int> ImgProcessor::ComputeHistogram(cv::InputArray in)
{
    cv::Mat mat(in.size(), in.type());
    in.copyTo(mat);

    std::vector<int> histogram(_centers.size(), 0);

    for(int row = 0; row < mat.rows; row++)
    {
        for(int col = 0; col < mat.cols; col++)
        {
            int clusterId = mat.at<int>(row, col);
            histogram[clusterId]++;
        }
    }

    return histogram;
}

/*  ---------------------------------------------------------
    Color change methods
    ---------------------------------------------------------*/
void ImgProcessor::SubstituteColor(cv::Vec3b color, cv::Vec3b newColor, cv::Mat &img){

    int r, g, b;
    r = color[2];
    g = color[1];
    b = color[0];

    int rTransform, gTransform, bTransform;
    rTransform = newColor[2] - r;
    gTransform = newColor[1] - g;
    bTransform = newColor[0] - b;

    int colorClusterId = GetClusterId(color, _centers);
    for(int row = 0; row < img.rows; row++)
    {
        for(int col = 0; col < img.cols; col++)
        {
            if(_labels.at<int>(row, col) == colorClusterId){

                cv::Vec3b colorVec = img.at<cv::Vec3b>(row, col);
                r = colorVec[2];
                g = colorVec[1];
                b = colorVec[0];

                uchar newR = std::max(std::min(r + rTransform, 255), 0);
                uchar newG = std::max(std::min(g + gTransform, 255), 0);
                uchar newB = std::max(std::min(b + bTransform, 255), 0);
                cv::Vec3b newColorVec = cv::Vec3b(newB, newG, newR);
                
                img.at<cv::Vec3b>(row, col) = newColorVec;
            }
        }
    }
}

void ImgProcessor::HueShift(cv::Vec3b color, cv::Vec3b newColor, cv::Mat &img){        

    int r, g, b, nr, ng, nb;
    r = color[2];
    g = color[1];
    b = color[0];
    nr = newColor[2];
    ng = newColor[1];
    nb = newColor[0];

    cv::Mat3b newColorHsvMat, ColorHsvMat; 
    cv::Mat3b newColorBGRMat(cv::Vec3b(nb, ng, nr));
    cv::cvtColor(newColorBGRMat, newColorHsvMat, cv::COLOR_BGR2HSV);
    cv::Mat3b ColorBGRMat(cv::Vec3b(b, g, r));
    cv::cvtColor(ColorBGRMat, ColorHsvMat, cv::COLOR_BGR2HSV);

    int colorHue = ColorHsvMat[0][0][0];
    int newColorHue = newColorHsvMat[0][0][0];
    int hueTransform = newColorHue - colorHue; 

    cv::Mat imgHSV;
    cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);
    int colorClusterId = GetClusterId(color, _centers);
    for(int row = 0; row < img.rows; row++)
    {
        for(int col = 0; col < img.cols; col++)
        {
            if(_labels.at<int>(row, col) == colorClusterId){
                cv::Vec3b colorVec = imgHSV.at<cv::Vec3b>(row, col);
                int hue = colorVec[0];
                int sat = colorVec[1];
                int val = colorVec[2];

                uchar newHue = newColorHue; 
                // uchar newHue = (hue + hueTransform)%180; 
                cv::Vec3b newColorVec = cv::Vec3b(newHue, sat, val);
                
                imgHSV.at<cv::Vec3b>(row, col) = newColorVec;
            }
        }
    }

    cv::cvtColor(imgHSV, img, cv::COLOR_HSV2BGR);
}

int ImgProcessor::GetClusterId(const cv::Vec3b sample, const std::vector<cv::Vec3b> centers){
    int clusterId, distance, minDistance;
    minDistance = ComputeDistance(sample, centers.at(0));
    clusterId = 0;

    for(int i = 1; i<centers.size(); i++){
        distance = ComputeDistance(sample, centers.at(i));
        if(distance < minDistance){
            minDistance = distance;
            clusterId = i;
        }
    }

    return clusterId;
}

int ImgProcessor::ComputeDistance(const cv::Vec3b point1, const cv::Vec3b point2){
    int distance, distX, distY, distZ;

    distX = point1[0] - point2[0];
    distY = point1[1] - point2[1];
    distZ = point1[2] - point2[2];
    distance = sqrt(pow(distX, 2) + pow(distY, 2) + pow(distZ, 2));

    return distance;
}