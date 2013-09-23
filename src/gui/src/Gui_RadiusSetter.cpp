#include "Gui_RadiusSetter.hpp"

Gui_RadiusSetter::Gui_RadiusSetter(Gui_ProcessorHandler& processor_hand,const std::string str):
    Gui_OptionSetterBaseClass(processor_hand,str),
    m_adjust_min(m_processor_hand.getOptions().getMinMaxRad().first, 0.0, 9999.0, 1.0, 10.0, 0.0),
    m_adjust_max(m_processor_hand.getOptions().getMinMaxRad().second, 0.0, 9999.0, 1.0, 10.0, 0.0),
    m_spin_butt_min(m_adjust_min),
    m_spin_butt_max(m_adjust_max),
    m_check_butt(LABEL_CHECKBUTTON_MAX_RAD),
    m_lab_min(LABEL_MIN_RAD),
    m_lab_max(LABEL_MAX_RAD)
{
    set_tooltip_text(TOOLTIP_RAD_SETTER);
    m_check_butt.set_active(!m_processor_hand.getOptions().getHasMaxRad());
    this->m_vbox.add(m_hbox2);
    m_hbox2.add(m_spin_butt_min);
    m_hbox2.add(m_lab_min);
    m_hbox2.add(m_spin_butt_max);
    m_hbox2.add(m_lab_max);
    m_hbox2.add(m_check_butt);
    m_spin_butt_min.signal_value_changed().connect(sigc::mem_fun(*this,&Gui_RadiusSetter::on_MinRadChanged));
    m_spin_butt_max.signal_value_changed().connect( sigc::hide_return( sigc::mem_fun(*this,&Gui_RadiusSetter::setOption)));
    m_check_butt.signal_toggled().connect( sigc::hide_return( sigc::mem_fun(*this,&Gui_RadiusSetter::setOption)  ) );
}

bool Gui_RadiusSetter::updateOptions(){
    DEV_INFOS("Trying to send a processing option");
    int val_min = m_spin_butt_min.get_value();
    int val_max = m_spin_butt_max.get_value();
//
    if(val_min > val_max){
        m_spin_butt_max.set_value(val_min);
        val_max = val_min;
    }

    bool tmp_bool = m_opts.setMinMaxRad(std::pair<int,int>(val_min,val_max));
    bool auto_max_rad = m_check_butt.get_active();

    if(auto_max_rad){
        m_spin_butt_max.set_sensitive(false);
    }
    else{
        m_spin_butt_max.set_sensitive();
    }
    m_opts.setHasMaxRad(!auto_max_rad);

    return tmp_bool ;
}

void Gui_RadiusSetter::updateNFiles(int i){
    if(i == 0)
        this->hide_all();
    else
        this->show_all();
}
