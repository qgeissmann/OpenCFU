#ifndef STEP_FILTIPOSITION2D_H
#define STEP_FILTIPOSITION2D_H

#include "defines.hpp"
#include "Step_BaseClass.hpp"
#include "ContourFamily.hpp"

class Step_FiltIPosition2D : public Step_BaseClass
{
    public:
        Step_FiltIPosition2D(ProcessingOptions& opts):
            Step_BaseClass(opts),m_use_this_filter(true)
            {}

    protected:
        void process(const void* src);
        void updateParams(const void* src,bool was_forced);
        bool needReprocess(const void* src);
        std::vector<int>  filter(const Result& in_numerical_result);

    void makeROIPolygons();
    private:
        bool m_use_this_filter;
        cv::Mat m_old_mask;
        cont_chunk m_ROIS_polygs;
        std::vector<int> m_ROIS_val;
        Result m_step_numerical_result;
};

#endif // STEP_FILTHS_H


