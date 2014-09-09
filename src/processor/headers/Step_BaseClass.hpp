#ifndef STEPBASECLASS_H
#define STEPBASECLASS_H

#include "defines.hpp"
#include "Result.hpp"
#include "ProcessingOptions.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"


class Step_BaseClass
{
    public:
        Step_BaseClass(ProcessingOptions& opts);
        bool run(void*& inout,bool forceReprocess = false);

        virtual void process(const void* src) = 0;
        virtual ~Step_BaseClass();
    protected:
        bool isSameImage(const cv::Mat& img1,const cv::Mat& img2);
        virtual void updateParams(const void* src,bool was_forced) = 0;
        virtual bool needReprocess(const void* src) = 0;
        ProcessingOptions& m_opts;
        void* m_step_result;
};

#endif // STEPBASECLASS_H
