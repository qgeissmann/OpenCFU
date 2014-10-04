#ifndef STEP1_H
#define STEP1_H

#include "defines.hpp"
#include "Step_BaseClass.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class Step_1: public Step_BaseClass
{
    public:
        Step_1(ProcessingOptions& opts):Step_BaseClass(opts){}
        void process(const void* src);

    protected:
        void updateParams(const void* src,bool was_forced);
        bool needReprocess(const void* src);
        void updateKernelSize();

    private:
        cv::Mat m_old_img;
        cv::Mat m_step_img;
        int m_old_min_rad;
        int m_kernel_size;
};

#endif // STEP1
