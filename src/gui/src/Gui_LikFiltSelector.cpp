#include "Gui_LikFiltSelector.hpp"

Gui_LikFiltSelector::Gui_LikFiltSelector(Gui_ProcessorHandler& processor_hand,const std::string str):
    Gui_OptionSetterBaseClass(processor_hand,str),
    m_adjust_thr(m_processor_hand.getOptions().getLikeThr(), 0.0, 99.9, 0.1, 3.0, 0.0),
    m_spin_butt_thr(m_adjust_thr),
    m_check_butt(LABEL_CHECKBUTTON_HAS_LIK_FILT),
    m_lab_thr(LABEL_THR)

{
    set_tooltip_text(TOOLTIP_LIK_FILT);
    m_spin_butt_thr.set_digits(1);
    m_check_butt.set_active(m_processor_hand.getOptions().getHasOutlierFilt());
    m_vbox.pack_start(m_check_butt);
    m_vbox.pack_start(m_hbox2);
    m_hbox2.pack_start(m_spin_butt_thr);
    m_hbox2.pack_start(m_lab_thr);
    m_spin_butt_thr.signal_value_changed().connect( sigc::hide_return( sigc::mem_fun(*this,&Gui_LikFiltSelector::setOption)));
    m_check_butt.signal_toggled().connect(sigc::mem_fun(*this,&Gui_LikFiltSelector::on_activate_filter));

}


bool Gui_LikFiltSelector::updateOptions(){
    float value = m_spin_butt_thr.get_value();
    bool has_thr = m_check_butt.get_active();
    m_opts.setHasOutlierFilt(has_thr);
    if(has_thr){

        return m_opts.setLikeThr(value);
    }
    else
        return true;
}


void Gui_LikFiltSelector::updateNFiles(int i){
    if(i == 0)
        this->hide_all();
    else
        this->show_all();
    on_tick_box();
}
