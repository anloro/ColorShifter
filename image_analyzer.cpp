/**
 * @file   image_analyzer.cpp
 * @brief  Functions to analyze the colours of an image.
 * @author Ángel Lorente Rogel
 * @date   21/11/2021
 */

#include "image_analyzer.h"
#include "utils.h"

std::vector<cv::Vec3b> BGRImgAnalyzer::GetColorPalette(const cv::Mat &img){
    // Get these from a configuration file
    int paletteSize = 5;
    int gridSize = 3;

    _table.GenerateColorTable(gridSize);
    std::map<cv::Point, int> clusters = FixedSizeGridClusterization(img, gridSize);
    std::vector<int> hist = ComputeHistogram(clusters);
    std::vector<int> ind = get_sorted_indexes<int>(hist);
    std::vector<cv::Vec3b> palette;

    // Get the palette
    int b, g, r;
    for(int i = 0; i < paletteSize; i++){
        std::string color = std::to_string(ind.at(i));
        _table.GetRGB(color, b, g, r);
        printf("b %i, g %i, r %i \n", b, g, r);
        palette.push_back(cv::Vec3b(b, g, r));
    }

    return palette;
}

std::vector<int> BGRImgAnalyzer::ComputeHistogram(const std::map<cv::Point, int> clusters)
{      
    std::vector<int> histogram(clusters.size(),0);

    for(auto const& n: clusters)
    {
        histogram[n.second]++;
    }

    return histogram;
};

std::map<cv::Point, int> BGRImgAnalyzer::FixedSizeGridClusterization(const cv::Mat &img, const int gridSize)
{
    std::vector<cv::Mat> channels;
    cv::split(img,channels);

    int gridSize_2 = gridSize*gridSize;
    int gridSize_3 = gridSize*gridSize*gridSize;
    double binSize = 256.0 / gridSize; // BGR range is [0, 255]

    std::map<cv::Point, int> clusters;

    // Clusterization using a grid of fixed size
    for(int i = 0; i < img.rows; i++)
    {
        for(int j = 0; j < img.cols; j++)
        {
            // OpenCV reads image in BGR format
            int r = channels[2].at<uchar>(i, j);
            int g = channels[1].at<uchar>(i, j);
            int b = channels[0].at<uchar>(i, j);

            int cluster_r = (int)(floor(r/binSize)); // bounded between [0, gridSize-1]
            int cluster_g = (int)(floor(g/binSize));
            int cluster_b = (int)(floor(b/binSize));

            int cluster_id =  cluster_r + cluster_g*gridSize + cluster_b*gridSize_2; // [0, gridSize^3 -1]
            clusters.insert(std::pair<cv::Point, int>(cv::Point(i,j), cluster_id)); // (pixel, cluster_id)
        }
    }

    std::vector<cv::Mat>().swap(channels);
    return clusters;
};

// std::vector<int> HSVImgAnalyzer::ComputeHistogram(const cv::Mat &img, const int gridSize) 
// {

//     return histogram;
// };

ImgAnalyzer* ImgAnalyzerFactory::GetAnalyzer(){
    // Get this option from a configuration file
    int option = 1;

    switch (option)
    {
        case 1:
            product = new BGRImgAnalyzer;
            break;
        case 2:
            product = nullptr;
            // product = new HSVImgAnalyzer;
            break;
        default:
            product = nullptr;
            break;
    }

    return product;
};
