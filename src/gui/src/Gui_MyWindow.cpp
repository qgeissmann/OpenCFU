#include "Gui_MyWindow.hpp"



Gui_MyWindow::Gui_MyWindow(Gui_ProcessorHandler& processor_hand, Gui_ConfigIO& config):
     Gtk::Window(),

     m_processor_hand(processor_hand),

     m_controlPanel(m_processor_hand,m_result_map,config),
     m_draw_frame(m_processor_hand,m_controlPanel.signal_show_decorator(),m_controlPanel.signal_show_mask()),
     m_result_frame(m_processor_hand,m_controlPanel.signal_set_to_NA(),m_result_map),


     m_Cursor_wait (Gdk::WATCH)
{
    DEV_INFOS(Glib::get_user_config_dir ());

    this->set_title(PROGRAM_NAME);

    this->set_border_width(10);
    m_v_paned.set_border_width(5);


    this->add(m_hbox);
    m_hbox.pack_start(m_controlPanel,false,false);

    m_hbox.pack_start(m_v_paned);
    m_v_paned.pack1(m_draw_frame);
    m_v_paned.pack2(m_result_frame);

    this->show_all_children();
    this->show();


    /* Change the cursor when processing */
    m_processor_hand.signal_state().connect( sigc::mem_fun(*this,&Gui_MyWindow::changeCursor));

    /*For drag and drop*/
    std::list<Gtk::TargetEntry> listTargets;
    listTargets.push_back(Gtk::TargetEntry("text/uri-list"));
    drag_dest_set(listTargets, Gtk::DEST_DEFAULT_MOTION | Gtk::DEST_DEFAULT_DROP, Gdk::ACTION_COPY | Gdk::ACTION_MOVE);
    signal_drag_data_received().connect(sigc::mem_fun(*this, &Gui_MyWindow::on_dropped_file));
    m_result_frame.signal_object_selected().connect(sigc::mem_fun(m_draw_frame, &Gui_DisplayFrame::on_show_specific_object));
    m_draw_frame.signal_click_on_obj().connect(sigc::mem_fun(m_result_frame, &Gui_ResultFrame::on_click_object)) ;
    m_ref_window = get_window();
}


//
void Gui_MyWindow::on_realize(){
    DEV_INFOS("window real");
    Gtk::Window::on_realize();

}


bool Gui_MyWindow::on_delete_event(GdkEventAny* event){
    DEV_INFOS("window real");

    if (m_result_frame.isAllSaved()){
        return false;
        }
    else{
        Gtk::MessageDialog dial(NO_SAVE_WARNING_MAIN, true, Gtk::MESSAGE_WARNING,  Gtk::BUTTONS_YES_NO );
        dial.set_secondary_text(NO_SAVE_WARNING_SECOND,true);
        dial.set_transient_for(*this);
//        *((Gtk::Window*) m_parent_widg->get_toplevel())
        if (dial.run() == Gtk::RESPONSE_YES)
            return false;
        else
            return true;
    }
}

void Gui_MyWindow::changeCursor(int state){
    if(state == PROCESSOR_IDLE_SIGNAL){
        m_ref_window->set_cursor(m_Cursor_basic);
    }
    else
        m_ref_window->set_cursor(m_Cursor_wait);

}


void Gui_MyWindow::on_dropped_file(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time){
    std::vector<std::string> paths;
    DEV_INFOS("D");
    if ((selection_data.get_length() >= 0) && (selection_data.get_format() == 8)){
        std::vector<Glib::ustring> file_list;
        file_list = selection_data.get_uris();
        for (auto &str: file_list){
            paths.push_back(str);
        }
        context->drag_finish(true, false, time);
    }
    else
        context->drag_finish(false, false, time);
    m_signal_drag_and_drop.emit(paths);

}

