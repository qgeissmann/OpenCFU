#ifndef CONTOURSPLITER_H
#define CONTOURSPLITER_H

#include "ContourFamily.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class ContourSpliter
{
    public:
        void split(std::vector<ContourFamily>& contour_fams, std::vector<signed char>& categ);//,std::vector<int>& n_per_clust);
        void splitOneCont(const ContourFamily& in, std::vector<ContourFamily>& out);
        void findPeaks(const cv::Mat& binary, cv::Mat& distance_map, cont_chunk& peaks_conts);
        void makeWatershedLabel(const cv::Mat& binary, const cont_chunk& peaks_conts, cv::Mat& labels);
        void watershedLike(cv::Mat &mask,cv::Mat& gray,int nlabs,double maxAreaModif);
};

#endif // CONTOURSPLITER_H
