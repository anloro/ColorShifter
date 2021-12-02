/**
 * @file   main.cpp
 * @brief  I will find the colour palette of a image.
 * @author Ángel Lorente Rogel
 * @date   01/11/2021
 */

#include <iostream>
#include <string>
#include <map>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "utils.h"
#include "image_processor.h"


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

    std::string imgPath = "/home/angel/Pictures/palette5.png";
    std::size_t found = imgPath.find_last_of("/\\");
    std::string path = imgPath.substr(0,found+1);
    std::string name = imgPath.substr(found+1);
    cv::imwrite(path + "palette_" + name, img);

    cv::imshow("image", img);
    cv::waitKey(0);
    cv::destroyWindow("image");
}

cv::Mat getImgWithPalette(const std::vector<cv::Vec3b> colors, const cv::Mat &img){
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

void SaveImg(const std::string imgPath, const cv::Mat img){
    std::size_t found = imgPath.find_last_of("/\\");
    std::string path = imgPath.substr(0,found+1);
    std::string name = imgPath.substr(found+1);
    cv::imwrite(path + "palette_" + name, img);
}

int main(int argc, char** argv)
{
    // Input and image preprocessing
    std::string imgPath = argv[1];
    // std::string imgPath = "/home/angel/Pictures/palette5.png";
    cv::Mat img = cv::imread(imgPath);

    if(img.empty()){
        std::cout << "Could not find the input image!" << std::endl;
        return -1;
    }

    double width = 1920.0/1.4;
    double height = 1080.0/1.4;
    double scale = std::min(width/img.cols, height/img.rows);
	cv::resize(img.clone(), img, cv::Size(), scale, scale, cv::INTER_LINEAR);

    // Image analysis and proccessing
    ImgProcessor analyzer = ImgProcessor();
    analyzer.ProcessImg(img);
    // get the palette
    std::vector<cv::Vec3b> palette = analyzer.GetColorPalette();

    cv::Mat palette_img = getImgWithPalette(palette, img);
    cv::imshow("image with palette", palette_img);
    // SaveImg(imgPath, palette_img);

    // change single color
    int indColor = 2;
    cv::Mat imgNewColor = img.clone();
    cv::Vec3b newColor =  cv::Vec3b(0, 255, 255); 
    analyzer.HueShift(palette[indColor], newColor, imgNewColor);
    // analyzer.SubstituteColor(palette[indColor], newColor, imgNewColor);
    auto newPalette = palette; // vector assignment makes a copy
    newPalette[indColor] = newColor;
    palette_img = getImgWithPalette(newPalette, imgNewColor);
    cv::imshow("image with new palette hue", palette_img);

    // change complete palette
    // std::vector<cv::Vec3b> newPalette;
    // newPalette.push_back(cv::Vec3b(42, 42, 42));
    // newPalette.push_back(cv::Vec3b(127, 42, 42));
    // newPalette.push_back(cv::Vec3b(212, 212, 127));
    // newPalette.push_back(cv::Vec3b(212, 127, 127));
    // newPalette.push_back(cv::Vec3b(212, 212, 212));
    // for(int i=0; i<palette.size(); i++){
    //     analyzer->HueShift(palette[i], newPalette[i], img);
    // }

    cv::waitKey(0);
    cv::destroyWindow("image with palette");
    cv::destroyWindow("image with new palette hue");

    return 0;
}