#include "Gui_ColourSelector.hpp"

Gui_ColourSelector::Gui_ColourSelector(Gui_ProcessorHandler& processor_hand,const std::string str):
    Gui_OptionSetterBaseClass(processor_hand,str),
    m_adjust_hue_center(m_processor_hand.getOptions().getCenTolHue().first, 0.0, 358, 2.0, 10.0, 0.0),
    m_adjust_hue_tol(m_processor_hand.getOptions().getCenTolHue().second, 0.0, 180, 1.0, 10.0, 0.0),
    m_adjust_sat_min(m_processor_hand.getOptions().getMinMaxSat().first, 0.0, 255, 1.0, 10.0, 0.0),
    m_adjust_sat_max(m_processor_hand.getOptions().getMinMaxSat().second, 0.0, 255, 1.0, 10.0, 0.0),
    m_spin_butt_hue_center(m_adjust_hue_center),
    m_spin_butt_hue_tol(m_adjust_hue_tol),
    m_spin_butt_sat_min(m_adjust_sat_min),
    m_spin_butt_sat_max(m_adjust_sat_max),
    m_check_butt(LABEL_CHECKBUTTON_HAS_HUE_FILT),
    m_lab_hue(LABEL_HUE),
    m_lab_center(LABEL_HUE_CENT),
    m_lab_tol(LABEL_HUE_TOL),
    m_lab_sat(LABEL_SATUR),
    m_lab_sat_min(LABEL_SATUR_MIN),
    m_lab_sat_max(LABEL_SATUR_MAX)
{
    set_tooltip_text(TOOLTIP_COLOUR_SELECTOR);
    m_hbox21.set_tooltip_text(TOOLTIP_COLOUR_SELECTOR_HUE);
    m_hbox22.set_tooltip_text(TOOLTIP_COLOUR_SELECTOR_SAT);


    m_vbox.pack_start(m_check_butt);
    m_vbox.pack_start(m_hbox2);
    m_hbox2.pack_start(m_col_wheel,true,true);
    m_hbox2.pack_start(m_vbox2,false,false);
    m_vbox2.pack_start(m_hbox21,false,false);

    m_hbox21.pack_start(m_lab_hue,false,false);
    m_hbox21.pack_start(m_spin_butt_hue_center,false,false);
    m_hbox21.pack_start(m_lab_center,false,false);
    m_hbox21.pack_start(m_spin_butt_hue_tol,false,false);
    m_hbox21.pack_start(m_lab_tol,false,false);

    m_vbox2.pack_start(m_hbox22);

    m_hbox22.pack_start(m_lab_sat);
    m_hbox22.pack_start(m_spin_butt_sat_min);
    m_hbox22.pack_start(m_lab_sat_min);
    m_hbox22.pack_start(m_spin_butt_sat_max);
    m_hbox22.pack_start(m_lab_sat_max);
    m_spin_butt_hue_center.set_wrap(true);

    m_spin_butt_hue_center.signal_value_changed().connect( sigc::hide_return( sigc::mem_fun(*this,&Gui_ColourSelector::setOption)));
    m_spin_butt_hue_tol.signal_value_changed().connect( sigc::hide_return( sigc::mem_fun(*this,&Gui_ColourSelector::setOption)));
    m_spin_butt_sat_min.signal_value_changed().connect( sigc::hide_return( sigc::mem_fun(*this,&Gui_ColourSelector::on_MinSatChanged)));
    m_spin_butt_sat_max.signal_value_changed().connect( sigc::hide_return( sigc::mem_fun(*this,&Gui_ColourSelector::setOption)));
    m_check_butt.signal_toggled().connect(sigc::mem_fun(*this,&Gui_ColourSelector::on_activate_filter));

}



bool Gui_ColourSelector::updateOptions(){
    DEV_INFOS("Trying to send a processing option");
    int val_cent = m_spin_butt_hue_center.get_value();
    int val_tol = m_spin_butt_hue_tol.get_value();

    int val_min = m_spin_butt_sat_min.get_value();
    int val_max = m_spin_butt_sat_max.get_value();

    if(val_min > val_max){
        m_spin_butt_sat_max.set_value(val_min);
        val_max = val_min;
    }

    bool has_hue = m_check_butt.get_active();
    m_opts.setHasHueFilt(has_hue);
    if(has_hue){
        bool success = m_opts.setCenTolHue(std::pair<int,int>(val_cent,val_tol));
        success = success && m_opts.setMinMaxSat(std::pair<int,int>(val_min,val_max));
        m_col_wheel.setParams(val_cent,val_tol,val_min,val_max);
        return success;
    }
    else
        return true;
}


void Gui_ColourSelector::updateNFiles(int i){
    if(i == 0)
        this->hide_all();
    else
        this->show_all();
    on_tick_box();
}
