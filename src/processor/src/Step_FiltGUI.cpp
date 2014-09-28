#include "Step_FiltGUI.hpp"

void Step_FiltGUI::process(const void* src){
    const Result& in_numerical_result(*((Result*)(src)));
    m_step_numerical_result = in_numerical_result;

    m_step_numerical_result.applyGuiFilter(m_filter_mat);

    m_step_numerical_result.setSameObjects(m_tag_same_result);
    m_step_result = ((void*) &m_step_numerical_result);
}


void Step_FiltGUI::updateParams(const void* src,bool was_forced){
    if(was_forced){
        const Result& res(*((Result*)(src)));
        m_filter_mat = cv::Mat (res.size(),1,CV_8S,cv::Scalar(-1));
        DEV_INFOS("DBG-> force filtgui to rework "<<m_filter_mat.rows);
        m_tag_same_result=false;
        }
    else{
        DEV_INFOS("DBG-> DO NOT forced filtgui to rework:\
            copy filter from options "<<m_opts.getGUIFilter().rows<<" vs "<<m_filter_mat.rows);
        m_opts.getGUIFilter().copyTo(m_filter_mat);
        m_tag_same_result=true;
    }
}

bool Step_FiltGUI::needReprocess(const void* src){
    return !isSameImage(m_opts.getGUIFilter(),m_filter_mat);
}
