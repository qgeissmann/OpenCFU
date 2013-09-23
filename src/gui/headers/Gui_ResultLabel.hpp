#ifndef GUI_RESULTLABEL_H
#define GUI_RESULTLABEL_H

#include <gtkmm/frame.h>
#include <gtkmm/box.h>
#include <gtkmm/separator.h>
#include <gtkmm/label.h>
#include <gtkmm/progressbar.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/spinbutton.h>
#include <giomm/file.h>
#include <sstream>
#include "text.hpp"
#include "Result.hpp"
#include "Gui_ProcessorHandler.hpp"
#include "ResultMap.hpp"

class Gui_ResultLabel: public Gtk::Frame
{
    public:
        Gui_ResultLabel(Gui_ProcessorHandler& processor_hand,ResultMap& result_map );
        void updateNFiles(int i);
        void updateLabel();
        sigc::signal<void,bool>& signal_set_to_NA(){
            return m_signal_set_to_NA;}
        sigc::signal<void,bool,double>& signal_show_decorator(){
            return m_signal_hide_show_decor;}

    protected:
        void on_processorHandSignal(int state);
        bool on_pulse();
        void on_show_deco();
        void on_set_NA();


    private:
        Gui_ProcessorHandler& m_processor_hand;
        Gtk::VBox m_vbox;
        Gtk::HBox m_hbox, m_hbox2;
        Gtk::HSeparator m_vsep;
        Gtk::Label m_lab_text;
        Gtk::Label m_lab_res;
        Gtk::Label m_lab_line_width;
        Gtk::ProgressBar m_progress_bar;
        Gtk::CheckButton m_check_butt_show_object;
        Gtk::CheckButton m_check_butt_NA;
        int m_state;
        sigc::signal<void,bool,double> m_signal_hide_show_decor;
        sigc::signal<void,bool > m_signal_set_to_NA;
        Gtk::Adjustment m_adjust_line_width;
        Gtk::SpinButton m_spin_butt_line_width;
        bool m_is_result_NA;
        Glib::RefPtr<Gio::File> m_last_file;
        ResultMap& m_result_map;
};

#endif // GUI_RESULTLABEL_H
