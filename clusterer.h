/**
 * @file   clusterization.h
 * @brief  .
 * @author Ángel Lorente Rogel
 * @date   27/11/2021
 */


#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/cvstd.hpp>
#include <map>

class Clusterer {
public:
    virtual void ComputeClusters(const cv::Mat &img, cv::InputOutputArray &labels, std::vector<cv::Vec3b> &centers) = 0;
protected:
    int _gridSize;
    cv::Mat _labels; // [pixelId, clusterId]
    std::vector<cv::Vec3b> _centers; // [b, g, r]
};

class BGRClusterer : public Clusterer{
public:
    BGRClusterer();
    void ComputeClusters(const cv::Mat &img, cv::InputOutputArray &labels, std::vector<cv::Vec3b> &centers) override;
private:
    void Initialize();
    void ComputeCenters();
    void FixedSizeGridClusterization(const cv::Mat &img);
    int GetClusterId(const int b, const int g, const int r);
};

class HueClusterer : public Clusterer{
public:
    HueClusterer();
    void ComputeClusters(const cv::Mat &img, cv::InputOutputArray &labels, std::vector<cv::Vec3b> &centers) override;
private:
    void Initialize();
    void ComputeCenters();
    void FixedSizeGridClusterization(const cv::Mat &img);
    int GetClusterId(const int hue);
};

class KMeansClusterer : public Clusterer{
public:
    void ComputeClusters(const cv::Mat &img, cv::InputOutputArray &labels, std::vector<cv::Vec3b> &centers) override;
};

class ClustererFactory {
public:
    // ~ClustererFactory(){delete product;}
    Clusterer* GetObject();

private:
    Clusterer* product;
};