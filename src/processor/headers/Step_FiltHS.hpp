#ifndef STEP_FILTHS_H
#define STEP_FILTHS_H

#include "defines.hpp"
#include "Step_BaseClass.hpp"
#include "ContourFamily.hpp"

class Step_FiltHS : public Step_BaseClass
{
    public:
        Step_FiltHS(ProcessingOptions& opts):
            Step_BaseClass(opts){}
    protected:
        void process(const void* src);
        void updateParams(const void* src,bool was_forced);
        bool needReprocess(const void* src);
        std::vector<bool>  filter(const Result& in_numerical_result);
    private:
        bool m_use_this_filter;
        int m_centr_hue;
        int m_min_sat;
        int m_tol_hue;
        int m_max_sat;
        Result m_step_numerical_result;
};

#endif // STEP_FILTHS_H

