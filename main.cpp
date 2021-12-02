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


cv::Mat GetImg(const std::string imgPath){
    cv::Mat img = cv::imread(imgPath);

    if(img.empty()){
        std::cout << "Could not find the input image!" << std::endl;
        return img;
    }

    double width = 1920.0/1.6;
    double height = 1080.0/1.6;
    double scale = std::min(width/img.cols, height/img.rows);
	cv::resize(img.clone(), img, cv::Size(), scale, scale, cv::INTER_LINEAR);

    return img;
}

void SaveImg(const std::string imgPath, const cv::Mat img){
    std::size_t found = imgPath.find_last_of("/\\");
    std::string path = imgPath.substr(0,found+1);
    std::string name = imgPath.substr(found+1);
    cv::imwrite(path + "palette_" + name, img);
}

cv::Mat CreateImgWithPalette(const std::vector<cv::Vec3b> colors, const cv::Mat &img){
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

cv::Mat img;
std::vector<cv::Vec3b> palette;
int indColor = 0;
int maxIndex = 5;
int nColors = 1;
ImgProcessor analyzer;
std::string windowName = "Image"; 

static void ChangeColor(int, void*)
{
    // process the color change
    cv::Mat imgNewColor = img.clone();
    cv::Vec3b newColor =  cv::Vec3b(0, 0, 255);
    int maxIndColor = indColor + nColors;
    int boundedIndColor = std::min(maxIndColor, maxIndex); 
    auto newPalette = palette; // vector assignment makes a copy

    for (int i = indColor; i<boundedIndColor; i++){
        analyzer.HueShift(palette[i], newColor, imgNewColor);
        // analyzer.SubstituteColor(palette[indColor], newColor, imgNewColor);
        newPalette[i] = newColor;
    }
    
    // show the image and the new palette
    cv::Mat palette_img = CreateImgWithPalette(newPalette, imgNewColor);
    cv::imshow(windowName, palette_img);
}

int main(int argc, char** argv)
{
    // Input and image preprocessing
    std::string imgPath = argv[1];
    // std::string imgPath = "/home/angel/Pictures/palette5.png";
    img = GetImg(imgPath);
    if(img.empty()){
        return -1;
    }

    // Image analysis and proccessing
    analyzer = ImgProcessor();
    analyzer.ProcessImg(img);
    // get the palette
    palette = analyzer.GetColorPalette();
    cv::Mat palette_img = CreateImgWithPalette(palette, img);
    cv::imshow("image with palette", palette_img);
    // SaveImg(imgPath, palette_img);

    // // change color
    cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("Change color:", windowName, &indColor, maxIndex, ChangeColor);
    cv::createTrackbar("n colors:", windowName, &nColors, maxIndex, ChangeColor);
    ChangeColor(0, 0);

    cv::waitKey(0);
    cv::destroyWindow("image with palette");
    // cv::destroyWindow("image with new palette hue");
    cv::destroyWindow(windowName);

    return 0;
}