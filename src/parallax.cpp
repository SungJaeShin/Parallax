#include <eigen3/Eigen/Dense>
#include <list>
#include <algorithm>
#include <vector>
#include <numeric>
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

#include "../include/extractor.h"
#include "../include/descriptor.h"
#include "../include/outlier_rejection.h"

int main()
{
    // Road images
    std::string img1_path = "/home/sj/workspace/paper_ws/covins_up/src/parallax/img/cand_1_3.png";
    std::string img2_path = "/home/sj/workspace/paper_ws/covins_up/src/parallax/img/cand_1_25.png";
    cv::Mat img1 = cv::imread(img1_path, 1);
    cv::Mat img2 = cv::imread(img2_path, 1);

    // Initialization keypoints and descriptor 
    std::vector<cv::KeyPoint> img1_kpt, img2_kpt;
    std::vector<cv::Point2f> img1_2d, img2_2d;
    cv::Mat img1_des, img2_des;

    // Extract features and descriptors
    sift(img1, img1_kpt);
    sift(img2, img2_kpt);
    daisy(img1, img2, img1_kpt, img2_kpt, img1_des, img2_des);

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

    std::cout << "\033[1;32m================== Correspondence Pair using F =================\033[0m" << std::endl;
    std::cout << "final correspondence pair: " << final_matches.size() << std::endl;

    // Compute Optical Flow
    computeOpticalFlow(img1, img2, img1_2d, img2_2d, final_query_kpt, final_cand_kpt, final_matches);

    // Compute Parallax
    double avg_dx = 0;
    double avg_dy = 0;
    double avg_dist = 0;
    double max_dx = 0;
    double max_dy = 0;
    double max_dist = 0;
    computeParallax(avg_dx, avg_dy, avg_dist, max_dx, max_dy, max_dist, 
                    final_query_kpt, final_cand_kpt, final_matches);

    // Parallax Results    
    std::cout << "\033[1;34m================== Parallax Results =================\033[0m" << std::endl;
    std::cout << "max dx: " << max_dx << ", max dy: " << max_dy << ", max dist: " << max_dist << std::endl;
    std::cout << "avg dx: " << avg_dx << ", avg dy: " << avg_dy << ", avg_dist: " << avg_dist << std::endl;


    std::vector<cv::KeyPoint> filter_query_kpt, filter_cand_kpt;
    std::vector<cv::DMatch> filter_matches;
    outlierParallax(avg_dx, avg_dy, final_query_kpt, final_cand_kpt, final_matches,
                    filter_query_kpt, filter_cand_kpt, filter_matches);
    
    // Parallax filtering results
    std::cout << "\033[1;35m================== Parallax Filtering Results =================\033[0m" << std::endl;
    std::cout << "final filtered correspondence pair: " << filter_matches.size() << std::endl;

    // Final Img 
    cv::Mat outImg;
    cv::drawMatches(img1, img1_kpt, img2, img2_kpt, filter_matches, outImg);
    cv::imshow("final correspondence", outImg);
    cv::waitKey(0);

    return 0;
}


