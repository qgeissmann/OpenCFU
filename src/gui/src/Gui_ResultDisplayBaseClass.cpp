#include "Gui_ResultDisplayBaseClass.hpp"

Gui_ResultDisplayBaseClass::Gui_ResultDisplayBaseClass(sigc::signal<void,Glib::RefPtr<Gio::File>,int>& signal_update_result_map, ResultMap& result_map):
    m_signal_update_result_map(signal_update_result_map),
    m_result_map(result_map)
{
    set_has_tooltip();
    add(m_vbox);
    m_vbox.pack_start(m_label,false,false);
    m_vbox.pack_start(m_scrolled_window,true,true);

    m_vbox.pack_start(m_hbox,false,false);
    m_scrolled_window.add(m_tree_view);

}

void Gui_ResultDisplayBaseClass::on_realize(){

    m_signal_update_result_map.connect(sigc::mem_fun(*this,&Gui_ResultDisplayBaseClass::on_update_view));
    Gtk::Frame::on_realize();
}

Gui_ResultDisplayBaseClass::~Gui_ResultDisplayBaseClass()
{
}
