#ifndef GUI_MYWINDOW_H
#define GUI_MYWINDOW_H

#include "defines.hpp"

#include <gtkmm/window.h>
#include <gtkmm/paned.h>
#include <gdkmm/cursor.h>
#include <gdkmm/window.h>
#include <sigc++/sigc++.h>

#include "Gui_ResultFrame.hpp"

#include "Gui_ControlPanel.hpp"
#include "Gui_OptionSetterBaseClass.hpp"
#include "Processor.hpp"
#include "ProcessingOptions.hpp"
#include "Gui_ProcessorHandler.hpp"
#include "Gui_Display.hpp"
#include "Gui_ConfigIO.hpp"



class Gui_MyWindow: public Gtk::Window
{
    public:
        Gui_MyWindow(Gui_ProcessorHandler& processor_hand, Gui_ConfigIO& config);
        sigc::signal<void, std::vector<std::string> > signal_drag_and_drop(){
            return m_signal_drag_and_drop;
        }

    protected:
        void changeCursor(int state);
        void on_dropped_file(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time);
        void on_realize();
        bool on_delete_event(GdkEventAny* event);
    private:
        Gui_ProcessorHandler& m_processor_hand;
        Gui_ControlPanel m_controlPanel;
        Gui_DisplayFrame m_draw_frame;
        Gui_ResultFrame m_result_frame;


        ResultMap m_result_map;

        Gtk::HBox m_hbox;
        Gtk::VPaned m_v_paned;
        sigc::signal<void, std::vector<std::string> > m_signal_drag_and_drop;
        Glib::RefPtr <Gdk::Window> m_ref_window;
        Gdk::Cursor m_Cursor_wait;
        Gdk::Cursor m_Cursor_basic;



};

#endif // GUI_MYWINDOW_H
