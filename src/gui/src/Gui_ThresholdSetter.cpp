
#include "Gui_ThresholdSetter.hpp"

Gui_ThresholdSetter::Gui_ThresholdSetter(Gui_ProcessorHandler& processor_hand,const std::string str):
    Gui_OptionSetterBaseClass(processor_hand,str),
    m_adjust(m_opts.getThr(), 0.0, 255.0, 1.0, 10.0, 0.0),
    m_spin_butt(m_adjust),
    m_check_butt(LABEL_CHECKBUTTON_AUTO_THR)
{
    set_tooltip_text(TOOLTIP_THR_SETTER);
    m_combo.set_title("Mode");

    m_combo_map["Regular"] = OCFU_THR_NORM;
    m_combo_map["Inverted"] = OCFU_THR_INV;
    m_combo_map["Bilateral"] = OCFU_THR_BILAT;

    m_combo.append_text("Regular");
    m_combo.append_text("Inverted");
    m_combo.append_text("Bilateral");

    m_combo.set_active(m_opts.getThrMode());
    m_check_butt.set_active(m_opts.getHasAutoThr());

    this->m_vbox.add(m_hbox2);
    m_hbox2.add(m_combo);
    m_hbox2.add(m_spin_butt);
    m_hbox2.add(m_check_butt);

    m_spin_butt.signal_value_changed().connect( sigc::hide_return( sigc::mem_fun(*this,&Gui_ThresholdSetter::setOption)));
    m_check_butt.signal_clicked().connect( sigc::hide_return( sigc::mem_fun(*this,&Gui_ThresholdSetter::setOption)));
    m_combo.signal_changed().connect( sigc::hide_return( sigc::mem_fun(*this,&Gui_ThresholdSetter::setOption)));
}

bool Gui_ThresholdSetter::updateOptions(){
    DEV_INFOS("Trying to send a processing option");
    int val = m_spin_butt.get_value();
    int mode = m_combo_map[m_combo.get_active_text()];
    bool auto_t = m_check_butt.get_active();

    if(auto_t)
        m_spin_butt.set_sensitive(false);
    else
        m_spin_butt.set_sensitive(true);

    bool tmp_bool = m_opts.setThr(val);
    bool tmp_bool2 = m_opts.setThrMode(mode);
    m_opts.setHasAutoThr(auto_t);

    return tmp_bool && tmp_bool2;
}

void Gui_ThresholdSetter::updateNFiles(int i){
    if(i == 0)
        this->hide_all();
    else
        this->show_all();
}
