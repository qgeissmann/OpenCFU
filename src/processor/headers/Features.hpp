#ifndef FEATURES_H
#define FEATURES_H

#include "ContourFamily.hpp"
#include "defines.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"


class Features
{
    public:
        Features();
        void calcFeatures(const ContourFamily& fam,cv::Mat out);
        int getNFeature(){return m_nfeatures;};
        static cv::Point2f calculateWH(const std::vector<cv::Point>& contour);
    protected:
        cv::Point2f calcHullPerimArea(const std::vector<cv::Point>& contour);
        inline static float calcTwoPointDist(const cv::Point P0,const cv::Point P1);
        void smoothContour(const std::vector<cv::Point>& in,std::vector<cv::Point>& out);
    private:
        const unsigned int m_nfeatures;
};


#endif // FEATURES_H
