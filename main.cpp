/**
 * @file   main.cpp
 * @brief  I will find the colour palette of a image.
 * @author Ángel Lorente Rogel
 * @date   01/11/2021
 */

#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <numeric> 
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "colorTable.h"
#include "utils.h"


std::vector<int> ComputeHistogram(const cv::Mat &img, const int gridSize) 
{
    std::vector<cv::Mat> channels;
    cv::split(img,channels);

    int gridSize_2 = gridSize*gridSize;
    int gridSize_3 = gridSize*gridSize*gridSize;

    double binSize = 256.0 / gridSize;        
    std::vector<int> histogram(gridSize_3,0);

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

            int cluster_n =  cluster_r + cluster_g*gridSize + cluster_b*gridSize_2; // [0, gridSize^3 -1]
            histogram[cluster_n]++;
        }
    }

    std::vector<cv::Mat>().swap(channels);
    return histogram;
}

void showPalette(std::vector<cv::Vec3b> colors){
    int size = 150;
    int n_colors = colors.size();
    cv::Mat img = cv::Mat(cv::Size(size*n_colors, size), CV_8UC3, cv::Scalar(0,0,0));

    img.setTo(cv::Scalar(255,0,0));

    for(int i=0; i<n_colors; i++){
        cv::Mat block = img.colRange(i*size, (i+1)*size);
        block.setTo(colors[i]);
        block = cv::Mat();
        // block.setTo(cv::Scalar(colors[i][0],colors[i][1],colors[i][2]));
    }

    cv::imshow("image", img);
    cv::waitKey(0);
    cv::destroyWindow("image");
}

cv::Mat getPaletteImg(std::vector<cv::Vec3b> colors, const cv::Mat &img){
    int n_colors = colors.size();
    int size = (int)round(img.cols/n_colors);
    cv::Mat canvas = cv::Mat(cv::Size(img.cols, img.rows + size), CV_8UC3, cv::Scalar(0,0,0));

    canvas.setTo(cv::Scalar(255,255,255));
    img.rowRange(0, img.rows).copyTo(canvas.rowRange(0, img.rows));

    for(int i=0; i<n_colors; i++){
        cv::Mat block = canvas(cv::Range(img.rows + 15, canvas.rows - 15), cv::Range(i*size +15, (i+1)*size -15));
        block.setTo(colors[i]);
        block = cv::Mat();
    }

    return canvas;
}

void changeColor(cv::Mat &img, int gridSize, int cluster, Color newColor, ColorTable &table){

    std::vector<cv::Mat> channels;
    cv::split(img,channels);

    int gridSize_2 = gridSize*gridSize;
    int gridSize_3 = gridSize*gridSize*gridSize;

    double binSize = 256.0 / gridSize;        

    int c_r, c_g, c_b, t_r, t_g, t_b;
    table.GetRGB(std::to_string(cluster), c_b, c_g, c_r);
    t_r = newColor.r - c_r; 
    t_g = newColor.g - c_g; 
    t_b = newColor.b - c_b;

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

            int cluster_n =  cluster_r + cluster_g*gridSize + cluster_b*gridSize_2; // [0, gridSize^3 -1]

            if(cluster == cluster_n){
                uchar n_r = std::max(std::min(r + t_r, 255), 0); 
                uchar n_g = std::max(std::min(g + t_g, 255), 0);                
                uchar n_b = std::max(std::min(b + t_b, 255), 0);
                cv::Vec3b c = cv::Vec3b(n_b, n_g, n_r);
                img.at<cv::Vec3b>(cv::Point(j, i)) = c;
            }
        }
    }

    std::vector<cv::Mat>().swap(channels);
}


void hueShift(cv::Mat &img, int gridSize, int cluster, Color newColor, ColorTable &table){

    std::vector<cv::Mat> channels;
    cv::split(img,channels);

    int gridSize_2 = gridSize*gridSize;
    int gridSize_3 = gridSize*gridSize*gridSize;

    double binSize = 256.0 / gridSize;        

    cv::Mat3b newColorHsvMat, cluster_c; 
    cv::Mat3b newColorM(cv::Vec3b(newColor.b, newColor.g, newColor.r));
    cv::cvtColor(newColorM, newColorHsvMat, cv::COLOR_BGR2HSV);

    int c_r, c_g, c_b;
    table.GetRGB(std::to_string(cluster), c_b, c_g, c_r);
    cv::cvtColor(cv::Mat3b(cv::Vec3b(c_b, c_g, c_r)), cluster_c, cv::COLOR_BGR2HSV);

    int hue_t = newColorHsvMat[0][0][0] - cluster_c[0][0][0]; 

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

            int cluster_n =  cluster_r + cluster_g*gridSize + cluster_b*gridSize_2; // [0, gridSize^3 -1]

            if(cluster == cluster_n){
                cv::Mat3b pixelColorHsv, newPixelColorBGR;
                cv::cvtColor(cv::Mat3b(cv::Vec3b(b, g, r)), pixelColorHsv, cv::COLOR_BGR2HSV);
                int pixelHue = pixelColorHsv[0][0][0];
                int newHue = (pixelHue + hue_t)%180;
                pixelColorHsv[0][0][0] = newColorHsvMat[0][0][0];
                cv::cvtColor(pixelColorHsv, newPixelColorBGR, cv::COLOR_HSV2BGR);
                int n_b = newPixelColorBGR[0][0][0];
                int n_g = newPixelColorBGR[0][0][1];
                int n_r = newPixelColorBGR[0][0][2];
                cv::Vec3b newPixelColor(n_b, n_g, n_r);
                img.at<cv::Vec3b>(cv::Point(j, i)) = newPixelColor;
            }
        }
    }

    std::vector<cv::Mat>().swap(channels);
}

int main(int argc, char** argv)
{
    ColorTable table;
    std::string imgPath = argv[1];
    // std::string imgPath = "/home/angel/Pictures/maxresdefault.jpg";
    cv::Mat img = cv::imread(imgPath);

    if(img.empty()){
        std::cout << "Could not find the input image!" << std::endl;
        return -1;
    }

    double width = 1920.0/1.3;
    double height = 1080.0/1.3;
    double scale = std::min(width/img.cols, height/img.rows);
	cv::resize(img.clone(), img, cv::Size(), scale, scale, cv::INTER_LINEAR);

    int gridSize = 3;
    table.GenerateColorTable(gridSize);
    std::vector<int> hist;
    hist = ComputeHistogram(img, gridSize);
    std::vector<int> ind = sort_indexes<int>(hist);
    std::vector<cv::Vec3b> palette;

    // get the palette
    int b, g, r;
    int paletteSize = 5;
    for(int i = 0; i < paletteSize; i++){
        std::string color = std::to_string(ind.at(i));
        table.GetRGB(color, b, g, r);
        printf("b %i, g %i, r %i \n", b, g, r);
        palette.push_back(cv::Vec3b(b, g, r));
    }

    cv::Mat palette_img = getPaletteImg(palette, img);
    cv::imshow("image with palette", palette_img);
    // cv::waitKey(0);
    // cv::destroyWindow("image with palette");

    std::size_t found = imgPath.find_last_of("/\\");
    std::string path = imgPath.substr(0,found+1);
    std::string name = imgPath.substr(found+1);
    // cv::imwrite(path + "palette_" + name, palette_img);

    // change the color
    // Color newColor =  Color{255, 189, 51}; 
    int index = 1;
    std::string color = std::to_string(ind.at(index));
    table.GetRGB(color, b, g, r);
    Color newColor =  Color{0, 0, 255}; 

    // cv::imshow("image", img);

    std::vector<Color> newPalette;
    newPalette.push_back(Color{42, 42, 42});
    newPalette.push_back(Color{127, 42, 42});
    newPalette.push_back(Color{212, 212, 127});
    newPalette.push_back(Color{212, 127, 127});
    newPalette.push_back(Color{212, 212, 212});

    for(int i = 0; i<paletteSize; i++){
        hueShift(img, gridSize, ind.at(i), newPalette[i], table);
    }

    // get the palette
    for(int i = 0; i < paletteSize; i++){
        std::string color = std::to_string(ind.at(i));
        table.GetRGB(color, b, g, r);
        printf("b %i, g %i, r %i \n", b, g, r);
        palette.push_back(cv::Vec3b(b, g, r));
    }

    palette_img = getPaletteImg(palette, img);
    cv::imshow("image with palette RGB", palette_img);

    for(int i = 0; i<paletteSize; i++){
        changeColor(img, gridSize, ind.at(i), newPalette[i], table);
    }

    // get the palette
    for(int i = 0; i < paletteSize; i++){
        std::string color = std::to_string(ind.at(i));
        table.GetRGB(color, b, g, r);
        printf("b %i, g %i, r %i \n", b, g, r);
        palette.push_back(cv::Vec3b(b, g, r));
    }

    palette_img = getPaletteImg(palette, img);
    cv::imshow("image with palette Hue", palette_img);

    // hueShift(img, gridSize, ind.at(index), newColor, table);
    // for(int i = 0; i < paletteSize; i++){
    //     hueShift(img, gridSize, ind.at(i), newColor, table);
    // }

    // cv::imshow("image modified RGB", img);
    cv::waitKey(0);
    // cv::destroyWindow("image");
    // cv::destroyWindow("image modified Hue");
    // cv::destroyWindow("image modified RGB");

    cv::destroyWindow("image");
    cv::destroyWindow("image with palette Hue");
    cv::destroyWindow("image with palette RGB");

    return 0;
}