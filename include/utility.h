#include <opencv2/opencv.hpp>

#define FLOW_BACK false

void drawimage(cv::Mat image1, std::vector<cv::KeyPoint> img1_kpt)
{
    // Draw image keypoints 
    cv::Mat outImg;
    cv::drawKeypoints(image1, img1_kpt, outImg);
    cv::imshow("Output", outImg);
}

std::vector<cv::KeyPoint> convert_Point2f_to_KeyPoint(std::vector<cv::Point2f> points_2d)
{
    std::vector<cv::KeyPoint> points_kpt;
    for(std::vector<cv::Point2f>::const_iterator it = points_2d.begin(); it != points_2d.end(); it++) 
    {
        cv::KeyPoint tmp(*it, 8);
        points_kpt.push_back(tmp);
    }
    return points_kpt;
}

// Code from VINS-Mono 
// Ref site: https://github.com/HKUST-Aerial-Robotics/VINS-Mono
void reduceVector(std::vector<cv::Point2f> &points_2d, std::vector<uchar> status)
{
    int j = 0;
    for (int i = 0; i < int(points_2d.size()); i++)
        if (status[i])
            points_2d[j++] = points_2d[i];
    points_2d.resize(j);
}

void reduceVector(std::vector<cv::KeyPoint> &points_kpt, std::vector<uchar> status)
{
    int j = 0;
    for (int i = 0; i < int(points_kpt.size()); i++)
        if (status[i])
            points_kpt[j++] = points_kpt[i];
    points_kpt.resize(j);
}

void reduceVector(std::vector<cv::DMatch> &matches, std::vector<uchar> status)
{
    int j = 0;
    for (int i = 0; i < int(matches.size()); i++)
        if (status[i])
            matches[j++] = matches[i];
    matches.resize(j);
}