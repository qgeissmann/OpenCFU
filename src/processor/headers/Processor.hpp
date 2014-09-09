#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "defines.hpp"
#include "opencv2/core/core.hpp"

#include "Step_BaseClass.hpp"
#include "ProcessingOptions.hpp"
#include "Predictor.hpp"
#include "Result.hpp"


class Processor
{
    public:
        Processor(ProcessingOptions& opts);
        ~Processor();
        void runAll();
        void setOptions(const ProcessingOptions& opts){
            m_opts = opts;
        }
        const Result& getNumResult()const {
            return *m_result;
        }
        void setIsBusy(bool b){m_is_busy = b;}
        bool getIsBusy(){return m_is_busy;}

        void writeResult();
    private:
        std::vector<Step_BaseClass*> m_processing_steps;
        ProcessingOptions& m_opts;
        Predictor m_predictor, m_predictor_ps;
        Result* m_result;
        cv::Mat m_raw_img;
        bool m_is_busy;
};

#endif // PROCESSOR_H
