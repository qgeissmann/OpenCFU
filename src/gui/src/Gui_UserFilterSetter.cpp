#include "Gui_UserFilterSetter.hpp"


Gui_UserFilterSetter::Gui_UserFilterSetter(Gui_ProcessorHandler& processor_hand):
    Gui_OptionSetterBaseClass(processor_hand,""),
    m_filter(processor_hand.getGUIFilter())
{
    m_opts.setGUIFilter(m_filter);
}

void Gui_UserFilterSetter::on_click_object(int obj, int but){

    if (but ==1 || obj < 0)
        return;

    if(but == 3){
        DEV_INFOS("User asked to switch the object "<<obj);
        m_idx_to_switch = obj;
        this->setOption();
    }

}


void Gui_UserFilterSetter::on_toggle_object(int obj, bool t){

        DEV_INFOS("User asked to switch the object "<<obj);
        m_idx_to_switch = obj;
        this->setOption();
}

bool Gui_UserFilterSetter::updateOptions(){

    const Result& result = m_processor_hand.getNumResult();
    const OneObjectRow& oor = result.getRow(m_idx_to_switch);
    // The results have deeply changed, we add renew vector
    if(!result.getSameObjects() || m_filter->empty()){
        cv::Mat(result.size(),1,CV_8S,cv::Scalar(-1)).copyTo(*m_filter);
    }

    if(oor.isValid()){
        m_filter->at<char>(m_idx_to_switch,0) = 0;
    }
     else{
        m_filter->at<char>(m_idx_to_switch,0) = 1;
    }
    return true;
}

