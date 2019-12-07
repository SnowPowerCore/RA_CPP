#include <iostream>
#include <thread>
#include <vector>

#include <omp.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>

#include "TimeTracker.h"

// NOTE: Сопоставление изображений по ключевым точкам с помощью OpenCV.

int main()
{
    const std::vector files = { "base.jpg", "locate.jpg" };
    std::vector<cv::Mat> images(files.size());
    std::vector<std::vector<cv::KeyPoint>> keypoints(files.size());
    std::vector<cv::Mat> descriptors(files.size());

    cv::Ptr<cv::FeatureDetector> detector = cv::ORB::create();
    cv::Ptr<cv::DescriptorExtractor> extractor = cv::ORB::create();

    {
        TimeTracker tt("Matching");

        // NOTE: Используем OpenMP для распараллеливания цикла.
        #pragma omp parallel for
        for (int i = 0; i < images.size(); ++i) {
            images[i] = cv::imread(files[i], cv::IMREAD_ANYCOLOR);

            detector->detect(images[i], keypoints[i]);
            extractor->compute(images[i], keypoints[i], descriptors[i]);

            std::cout << "thread: " << std::this_thread::get_id() << " "
                      << "image: " << i << " "
                      << "keypoints: " << keypoints[i].size() << " "
                      << "\n";
        }
    }

    cv::BFMatcher matcher(cv::NORM_HAMMING, true);

    std::vector<cv::DMatch> matches;

    matcher.match(
        static_cast<cv::OutputArray>(descriptors[0]),
        static_cast<cv::OutputArray>(descriptors[1]),
        matches
    );

    std::sort(matches.begin(), matches.end(), [](auto lhs, auto rhs) {
        return lhs.distance < rhs.distance;
    });

    if (matches.size() > 10) {
        matches.resize(10);
    }

    for (const cv::DMatch& match : matches) {
        std::cout << match.distance << ", ";
    }

    std::cout << "\n";

    cv::Mat output;
    cv::drawMatches(images[0], keypoints[0], images[1], keypoints[1], matches, output);

    cv::imwrite("matches.png", output);

    return 0;
}
