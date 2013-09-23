#ifndef STEP2_H
#define STEP2_H

#include "defines.hpp"
#include "Step_BaseClass.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class Step_2: public Step_BaseClass
{
    public:
        Step_2(ProcessingOptions& opts):
            Step_BaseClass(opts),
            MEDIAN_BLUR_MAX_BLOCKSIZE(200),
            LAPOFGAUSS_BLUR_SIZE(11){}
        void process(const void* src);

    protected:
        void updateParams(const void* src,bool was_forced);
        bool needReprocess(const void* src);
        void makeConvolvedMask();
        void SubstractLapOGauss(const cv::Mat& in, cv::Mat& out, int blurSize);
    private:
        cv::Mat m_old_mask;
        cv::Mat m_conv_mask;
        cv::Mat m_step_img;
        int m_old_thr_mode;
        int m_block_size;
        const int MEDIAN_BLUR_MAX_BLOCKSIZE;
        const int LAPOFGAUSS_BLUR_SIZE;
};

#endif // STEP2_H
