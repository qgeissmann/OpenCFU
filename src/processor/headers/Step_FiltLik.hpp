#ifndef STEP_FILTLIK_H
#define STEP_FILTLIK_H
#include "Step_BaseClass.hpp"

class Step_FiltLik : public Step_BaseClass
{
    public:
        Step_FiltLik(ProcessingOptions& opts):
            Step_BaseClass(opts),
            N_FEATURES(6),
            m_means(N_FEATURES),
            m_sds(N_FEATURES){}

    protected:
        std::vector<bool> filter(const Result& in_numerical_result);
        void process(const void* src);
        void updateParams(const void* src,bool was_forced);
        bool needReprocess(const void* src);
        float calcLikScore(std::vector<float>& in);

    private:
        const int N_FEATURES;
        bool m_use_this_filter;
        float m_lik_thr;
        std::vector<float> m_means;
        std::vector<float> m_sds;
        Result m_step_numerical_result;


};
#endif // STEP_FILTLIK_H

