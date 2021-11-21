// /**
//  * @file   main.cpp
//  * @brief  I will find the colour palette of a image.
//  * @author Ángel Lorente Rogel
//  * @date   01/11/2021
//  */

// #include <iostream>
// #include <string>
// #include <map>
// #include <algorithm>
// #include <opencv2/imgcodecs.hpp>
// #include <opencv2/core/cvstd.hpp>
// #include <opencv2/highgui.hpp>
// #include "colorTable.h"


// template<typename A, typename B>
// std::pair<B,A> flip_pair(const std::pair<A,B> &pair){
//     return std::pair<B,A>(pair.second, pair.first);
// }

// template<typename A, typename B>
// std::multimap<B,A> flipMap(const std::map<A, B> &src){
//     std::multimap<B,A> dst;
//     std::transform(src.begin(), src.end(), 
//                     std::inserter(dst, dst.begin()), flip_pair<A,B>);
//     return dst;
// }

// std::vector<int> ComputeHistogram(const cv::Mat &img, int gridSize) 
// {
//     cv::Mat channels[3];
//     cv::split(img,channels);

//     double binSize = 255.0 / gridSize;        
//     std::vector<int> histogram(gridSize^3,0);

//     for(int i = 0; i < img.rows; i++)
//     {
//         const uchar* r_i = channels[0].ptr<uchar>(i);
//         const uchar* g_i = channels[1].ptr<uchar>(i);
//         const uchar* b_i = channels[2].ptr<uchar>(i);
//         for(int j = 0; j < img.cols; j++)
//         {
//             int cluster_r = std::min((int)round((r_i[j]+1)/binSize), gridSize)-1; // bounded between [0, gridSize-1]
//             int cluster_g = std::min((int)round((g_i[j]+1)/binSize), gridSize)-1;
//             int cluster_b = std::min((int)round((b_i[j]+1)/binSize), gridSize)-1;
            
//             int cluster_n =  cluster_r + cluster_g*gridSize + cluster_b*gridSize^2; // [0, gridSize^3 -1]
//             histogram[cluster_n]++;
//         }
//     }

//     return histogram;
// }

// void showPalette(std::vector<cv::Vec3b> colors){
//     int size = 150;
//     int n_colors = colors.size();
//     cv::Mat img = cv::Mat(cv::Size(size*n_colors, size), CV_8UC3, cv::Scalar(0,0,0));

//     img.setTo(cv::Scalar(255,0,0));

//     for(int i=0; i<n_colors; i++){
//         cv::Mat block = img.colRange(i*150, (i+1)*150);
//         block.setTo(colors[i]);
//         // block.setTo(cv::Scalar(colors[i][0],colors[i][1],colors[i][2]));
//     }

//     cv::imshow("image", img);
//     cv::waitKey(0);
//     cv::destroyWindow("image");
// }

// int main(int argc, char** argv)
// {
//     ColorTable table;
//     std::string imgPath = argv[1];
//     // std::string imgPath = "/home/angel/Pictures/violin.jpg";
//     cv::Mat img = cv::imread(imgPath);

//     cv::imshow("image", img);
//     cv::waitKey(0);
//     cv::destroyWindow("image");

//     if(img.empty()){
//         std::cout << "Could not find the input image!" << std::endl;
//         return -1;
//     }

//     cv::Mat channels[3];
//     cv::split(img,channels);

//     int totalPixels = 0;
//     std::map<std::string,int> palette;
//     // Iterate over the image to get the color of each pixel
//     for(int i = 0; i < img.rows; i++)
//     {
//         const uchar* r_i = channels[0].ptr<uchar>(i);
//         const uchar* g_i = channels[1].ptr<uchar>(i);
//         const uchar* b_i = channels[2].ptr<uchar>(i);
//         for(int j = 0; j < img.cols; j++)
//         {
//             int value_r = r_i[j];
//             int value_g = g_i[j];
//             int value_b = b_i[j];

//             std::string color = table.LookUpColor(value_r, value_g, value_b);

//             if (palette.count(color) == 0)
//             {
//                 // The color is not in the palette yet
//                 palette.insert(std::make_pair(color, 1));
//             }
//             else
//             {
//                 int n = palette.at(color);
//                 palette.erase(color);
//                 palette.insert(std::make_pair(color, n+1));
//             }

//             totalPixels += 1;
//         }
//     }

//     std::multimap<int, std::string> palette_o = flipMap<std::string, int>(palette);
//     std::vector<cv::Vec3b> colors;
//     int n_colors = 8;
//     std::multimap<int, std::string>::iterator it = palette_o.end();
    
//     int r, g, b;
//     for(int i=0; i<n_colors; i++){
//         std::advance(it, -1);
//         table.GetRGB(it->second, r, g, b);
//         colors.push_back(cv::Vec3b(r,g,b));
//         // colors.push_back(cv::Vec3b(b,g,r));
//     }

//     showPalette(colors);

//     return 0;
// }