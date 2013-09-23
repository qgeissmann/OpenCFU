
#include "Gui_ResultLabel.hpp"

Gui_ResultLabel::Gui_ResultLabel(Gui_ProcessorHandler& processor_hand,ResultMap& result_map):
 m_processor_hand(processor_hand),
 m_lab_line_width(LABEL_SET_DECO_LINE_WIDTH),
 m_check_butt_show_object(RESULT_LIST_CHECKBUTTON),
 m_check_butt_NA("Set as NA"),
 m_adjust_line_width(1.0, 0.0, 10.0, 0.1,1.0, 0.0),
 m_spin_butt_line_width(m_adjust_line_width),
 m_is_result_NA(false),
 m_result_map(result_map)
{
    DEV_INFOS("building result label");
    set_has_tooltip();

    set_tooltip_text(TOOLTIP_RESULT_LAB);
    m_check_butt_NA.set_tooltip_text(TOOLTIP_RESULT_LAB_NA);
    m_check_butt_show_object.set_tooltip_text(TOOLTIP_RESULT_LAB_DECO);
    m_spin_butt_line_width.set_tooltip_text(TOOLTIP_RESULT_LAB_LINE_WIDTH);


    this->set_border_width(3);
    this->add(m_vbox);
    m_vbox.pack_start(m_hbox,false,false);
    m_vbox.pack_start(m_progress_bar,false,false);
    m_hbox.pack_start(m_lab_text);

    m_hbox.pack_end(m_check_butt_NA);
    m_hbox.pack_end(m_lab_res);

    m_vbox.pack_start(m_hbox2);
    m_hbox2.pack_start(m_check_butt_show_object);

    m_hbox2.pack_end(m_spin_butt_line_width);
    m_hbox2.pack_end(m_lab_line_width);
    m_check_butt_show_object.set_active();

    m_spin_butt_line_width.set_digits(1);

    m_processor_hand.signal_state().connect( sigc::mem_fun(*this,&Gui_ResultLabel::on_processorHandSignal));
    m_spin_butt_line_width.signal_value_changed().connect( sigc::mem_fun(*this ,&Gui_ResultLabel::on_show_deco));
    m_check_butt_show_object.signal_clicked().connect( sigc::mem_fun(*this ,&Gui_ResultLabel::on_show_deco));
    m_check_butt_NA.signal_clicked().connect( sigc::mem_fun(*this ,&Gui_ResultLabel::on_set_NA));
    DEV_INFOS("result label ok");
}

void Gui_ResultLabel::on_set_NA(){
    m_is_result_NA = m_check_butt_NA.get_active();
    m_signal_set_to_NA.emit(m_is_result_NA);
    updateLabel();
}


void Gui_ResultLabel::on_show_deco(){
    double lw = m_spin_butt_line_width.get_value();
    bool show = m_check_butt_show_object.get_active();

    if(show)
        m_spin_butt_line_width.set_sensitive(true);
    else
        m_spin_butt_line_width.set_sensitive(false);
    m_signal_hide_show_decor.emit(show,lw);
}
bool Gui_ResultLabel::on_pulse(){

    if(m_state == PROCESSOR_IDLE_SIGNAL)
        return false;
    else{
        m_progress_bar.pulse();
        return true;
    }
}

void Gui_ResultLabel::updateNFiles(int i){
    if(i == 0)
        this->hide_all();
    else
        this->show_all();
}

void Gui_ResultLabel::on_processorHandSignal(int state){
    if(m_state != state){
        m_state = state;
        updateLabel();

        std::string tmp_name  = m_processor_hand.getOptions().getImageName();
        Glib::RefPtr<Gio::File> file;
        file = Gio::File::create_for_path(tmp_name);
        DEV_INFOS(tmp_name);
        DEV_INFOS((int) !m_result_map.getIsFileNA(file));

        if( !file->equal(m_last_file)){
            if(!m_result_map.getIsFileNA(file))
                m_check_butt_NA.set_active(false);
            else
                m_check_butt_NA.set_active(true);
            m_last_file = file;
           }


    }
}

void Gui_ResultLabel::updateLabel(){
    const Result& result = m_processor_hand.getNumResult();



    std::stringstream ss;
    if(m_state != PROCESSOR_IDLE_SIGNAL){
        m_lab_text.set_markup(RESULT_LABEL_BUSY);
        m_progress_bar.show();
        Glib::signal_timeout().connect(sigc::mem_fun(*this,&Gui_ResultLabel::on_pulse), 33);
        m_check_butt_NA.hide();
    }
    else{
            DEV_INFOS((int)m_is_result_NA);
             m_lab_text.set_markup(RESULT_LABEL_IDLE);
        if(!m_is_result_NA){
            ss  <<"<span font_size=\"x-large\"><b>"
                <<result.getNValid()
                <<"</b></span><sub>/"
                <<result.size()<<"</sub>";
        }
        else{
            ss  <<"<span font_size=\"x-large\"><b>NA</b></span><sub>/NA</sub>";
        }
            m_progress_bar.set_fraction(1.0);
            m_check_butt_NA.show();
    }
    m_lab_res.set_markup(ss.str());

}
