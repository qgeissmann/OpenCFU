#include "Step_FiltHS.hpp"

void Step_FiltHS::process(const void* src){
    const Result& in_numerical_result(*((Result*)(src)));
    if(!m_use_this_filter){
       m_step_numerical_result = in_numerical_result;
    }
    else{
      m_step_numerical_result = in_numerical_result;
      m_step_numerical_result.applyFilter(filter(in_numerical_result));
    }
    m_step_result = ((void*) &m_step_numerical_result);
}


void Step_FiltHS::updateParams(const void* src,bool was_forced){
     m_use_this_filter = m_opts.getHasHueFilt();
     m_centr_hue = m_opts.getCenTolHue().first;
     m_min_sat = m_opts.getMinMaxSat().first;
     m_tol_hue = m_opts.getCenTolHue().second;
     m_max_sat = m_opts.getMinMaxSat().second;
}

bool Step_FiltHS::needReprocess(const void* src){
    bool toReprocess = false;
    toReprocess = ( m_use_this_filter != m_opts.getHasHueFilt() ||
                     m_centr_hue != m_opts.getCenTolHue().first ||
                     m_min_sat != m_opts.getMinMaxSat().first ||
                     m_tol_hue != m_opts.getCenTolHue().second||
                     m_max_sat != m_opts.getMinMaxSat().second);
    return toReprocess;
}


std::vector<bool> Step_FiltHS::filter(const Result& in_numerical_result){

    std::vector<bool> valid(in_numerical_result.size(),true);
    for(unsigned int i = 0; i < valid.size(); i++){
        const OneObjectRow& oor = in_numerical_result.getRow(i);
        cv::Scalar mean = oor.getBGRMean();
        cv::Mat one_pix(1,1,CV_8UC3,mean);
        cv::cvtColor(one_pix,one_pix,cv::COLOR_BGR2HLS);
        mean = cv::mean(one_pix);

        int mean_hue =((int) mean[0])*2;

        float aa = (float) mean_hue *  3.1416 /180;
        float bb = (float) m_centr_hue *  3.1416 /180;
        int diff = (int) fabs(atan2(sin(aa-bb), cos(aa-bb)) * 180 / 3.1416);
        int mean_sat = (int) mean[2];

        if(diff > m_tol_hue || mean_sat > m_max_sat || mean_sat < m_min_sat)
             valid[i] = false;
        else
            valid[i] = oor.isValid();
    }
    return valid;
}

