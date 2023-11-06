#include <eigen3/Eigen/Dense>
#include <list>
#include <algorithm>
#include <vector>
#include <numeric>
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

#include "extractor.h"
#include "descriptor.h"
#include "outlier_rejection.h"
#include "parallax.h"

void drawimage(cv::Mat image1, std::vector<cv::KeyPoint> img1_kpt)
{
    // Draw image keypoints 
    cv::Mat outImg;
    cv::drawKeypoints(image1, img1_kpt, outImg);
    cv::imshow("Output", outImg);
    cv::waitKey(0);
}

int main()
{
    // Road images
    std::string img1_path = "~/src/parallax/img/cand_1_3.png";
    std::string img2_path = "~/src/parallax/img/cand_1_35.png";
    cv::Mat img1 = cv::imread(img1_path, 1);
    cv::Mat img2 = cv::imread(img2_path, 2);

    // Initialization keypoints and descriptor 
    std::vector<cv::KeyPoint> img1_kpt, img2_kpt;
    std::vector<cv::Point2f> img1_2d, img2_2d;
    cv::Mat img1_des, img2_des;

    // Extract features and descriptors
    sift(img1, img1_kpt);
    sift(img2, img2_kpt);
    surf(img1, img2, img1_kpt, img2_kpt, img1_des, img2_des);

    // Feature correspondence using features and descriptors
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::BFMatcher::create(cv::NORM_L2, false);
    std::vector<std::vector<cv::DMatch>> matches;
    matcher -> knnMatch(img1_des, img2_des, matches, 2);

    // Final correspondence pair
    cv::Mat F;
    std::vector<cv::KeyPoint> final_query_kpt, final_cand_kpt;
    std::vector<cv::DMatch> final_matches;
    F = outlierRejection(img1_2d, img2_2d, img1_kpt, img2_kpt, matches, 1, 0.999, 
                         final_query_kpt, final_cand_kpt, final_matches);

    // Compute Parallax
    double avg_dx = 0;
    double avg_dy = 0;
    double avg_dist = 0;
    double max_dx = 0;
    double max_dy = 0;
    double max_dist = 0;
    computeParallax(avg_dx, avg_dy, avg_dist, max_dx, max_dy, max_dist, 
                    final_query_kpt, final_cand_kpt, final_matches);

    // Final debug    
    std::cout << "final correspondence pair: " << final_matches.size() << std::endl;
    std::cout << "[After Filtering]" << std::endl;
    std::cout << "max dx: " << max_dx << ", max dy: " << max_dy << ", max dist: " << max_dist << std::endl;
    std::cout << "avg dx: " << avg_dx << ", avg dy: " << avg_dy << ", avg_dist: " << avg_dist << std::endl;


    std::vector<cv::KeyPoint> filter_query_kpt, filter_cand_kpt;
    std::vector<cv::DMatch> filter_matches;
    outlierParallax(avg_dx, avg_dy, final_query_kpt, final_cand_kpt, final_matches,
                    filter_query_kpt, filter_cand_kpt, filter_matches);
    
    std::cout << "final filtered correspondence pair: " << filter_matches.size() << std::endl;


    // Final Img 
    cv::Mat outImg;
    cv::drawMatches(img1, img1_kpt, img2, img2_kpt, filter_matches, outImg);
    cv::imshow("final correspondence", outImg);
    cv::waitKey(0);

    return 0;
}


