
#include "Gui_ResultFrame.hpp"


Gui_ResultFrame::Gui_ResultFrame(Gui_ProcessorHandler& processor_hand, sigc::signal<void,bool>& signal_set_to_NA, ResultMap& result_map):
    m_result_map(result_map),
    m_processor_hand(processor_hand),
    m_result_display(m_signal_update_result_map, m_result_map),
    m_result_detail(m_signal_update_result_map,m_signal_select_object,m_signal_toggle_object, m_result_map),
    m_user_filter(m_processor_hand)
{
    DEV_INFOS("RESUlt frame");
    this->pack1(m_result_display);
    this->pack2(m_result_detail);
    processor_hand.signal_state().connect( sigc::mem_fun(*this,&Gui_ResultFrame::on_processor_idle));
    signal_set_to_NA.connect( sigc::mem_fun(*this,&Gui_ResultFrame::on_set_current_to_NA));
    m_signal_toggle_object.connect( sigc::mem_fun(m_user_filter,&Gui_UserFilterSetter::on_toggle_object));
//    signal_set_to_NA.connect( sigc::mem_fun(m_result_display,&Gui_ResultListDisplay::on_setToNA));
    DEV_INFOS("RESUlt frame OK");
}
void Gui_ResultFrame::on_click_object(int i,int but){
            DEV_INFOS("The mouse button "<< but <<" was clicked on the object "<<i);
            if(but == 1)
                m_result_detail.set_selection(i);


            m_user_filter.on_click_object(i,but);
        }

void Gui_ResultFrame::on_realize(){
    set_position(this->get_allocation().get_width() / 2);
    Gtk::HPaned::on_realize();
}
void Gui_ResultFrame::on_set_current_to_NA(bool NA){
    std::string tmp_name  = m_processor_hand.getOptions().getImageName();
    Glib::RefPtr<Gio::File> file;
    file = Gio::File::create_for_path(tmp_name);
    int where = m_result_map.setFileToNA(file,NA);
    m_signal_update_result_map.emit(file,where);
}

void Gui_ResultFrame::on_processor_idle(int state){
    if (state == PROCESSOR_IDLE_SIGNAL){

        std::string tmp_name  = m_processor_hand.getOptions().getImageName();
        Glib::RefPtr<Gio::File> file;
        file = Gio::File::create_for_path(tmp_name);
        const Result& tmp_res = m_processor_hand.getNumResult();
        int where = m_result_map.append(file,tmp_res);

        m_signal_update_result_map.emit(file,where);
    }


}
