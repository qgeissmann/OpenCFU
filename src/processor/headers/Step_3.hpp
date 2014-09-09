#ifndef STEP3_H
#define STEP3_H

#include "defines.hpp"
#include "Features.hpp"
#include "ContourFamily.hpp"
#include "Step_BaseClass.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "Predictor.hpp"



class Step_3: public Step_BaseClass
{
    public:
        Step_3(ProcessingOptions& opts,Predictor& predictor):
            Step_BaseClass(opts),
            m_predictor(predictor){
            }
        void process(const void* src);

    protected:
        void updateParams(const void* src,bool was_forced);
        bool needReprocess(const void* src);
        void makeFeaturesMatrix(const std::vector<ContourFamily>& contour_fams,cv::Mat& dst);
        void makeContourChunksVect(const cv::Mat& src, std::vector<ContourFamily>& contour_fams);
        void drawAllValid(cv::Mat& inout,std::vector<ContourFamily>& contour_fams,std::vector<signed char>& categ);
        void subsample(const std::vector<cv::Point>& in, std::vector<cv::Point>& out,const unsigned  int size_out = 50);
        bool isSizeOK(const std::vector<cv::Point>& cont);
        Predictor& m_predictor;
        Features m_featureMaker;
        int m_min_radius, m_max_radius,m_max_cont_width;
        bool m_has_max_radius;
        cv::Mat m_step_img;
};



#endif // STEP3_H
