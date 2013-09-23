#ifndef STEP4_H
#define STEP4_H
#include "Step_3.hpp"

#include "defines.hpp"
#include "Step_BaseClass.hpp"
#include "ContourFamily.hpp"
#include "ContourSpliter.hpp"

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class Step_4: public Step_3
{
    public:
        Step_4(ProcessingOptions& opts, Predictor& predictor,Predictor& predictor_ps, cv::Mat& raw_img):
            Step_3(opts,predictor),
            m_raw_img(raw_img),
            m_predictor_ps(predictor_ps){}
        void process(const void* src);

    protected:
        void makeContourChunksVect(const cv::Mat& src,std::vector<ContourFamily>& contour_fams);
        void preFilterContourSize(std::vector<ContourFamily>& contour_fams);
        void writeNumResults(const std::vector<ContourFamily>& contour_fams, const std::vector<signed char>& categ);//, const std::vector<int>& n_per_clust );
        void updateParams(const void* src,bool was_forced);
        bool needReprocess(const void* src);
        void separateUnsplited(const std::vector<ContourFamily>& contour_fams,std::vector<ContourFamily>& contour_fams_unsplit,std::vector<ContourFamily>& contour_fams_split);

    private:
        int m_threshold;
        bool m_has_auto_thr;
        ContourSpliter m_contour_spliter;
        cv::Mat& m_raw_img;
        Result m_step_numerical_result;
        Predictor& m_predictor_ps;

};



#endif //STEP4_H
