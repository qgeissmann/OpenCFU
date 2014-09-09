#ifndef STEP_FILTGUI_H
#define STEP_FILTGUI_H

#include "defines.hpp"
#include "Step_BaseClass.hpp"
#include "ContourFamily.hpp"


class Step_FiltGUI : public Step_BaseClass
{
    public:
        Step_FiltGUI(ProcessingOptions& opts):
        Step_BaseClass(opts),m_tag_same_result(false){}
    protected:
        void process(const void* src);
        void updateParams(const void* src,bool was_forced);
        bool needReprocess(const void* src);
    private:
        bool m_use_this_filter;
        cv::Mat m_filter_mat;
        Result m_step_numerical_result;
        bool m_tag_same_result;
};
#endif // STEP_FILTGUI_H
