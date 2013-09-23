#ifndef GUI_MYAPPLICATION_H
#define GUI_MYAPPLICATION_H
//

#include <gtkmm.h>
#include <gtkmm/main.h>
#include "Gui_ConfigIO.hpp"
#include "Processor.hpp"
#include "defines.hpp"
#include "Gui_MyWindow.hpp"
#include "Gui_ProcessorHandler.hpp"
#include "Gui_HelloWindow.hpp"
//


class Gui_MyApplication: public Gtk::Main
{
    public:
        Gui_MyApplication(int argc,char** argv,Processor& pro);
//
    private:
        Processor& m_processor;
        ProcessingOptions m_opts;
        Gui_ProcessorHandler m_pro_hand;
        Gui_ConfigIO m_config;
        std::vector<Glib::RefPtr<Gdk::Pixbuf> > m_icon_list;



};

#endif // GUI_MYAPPLICATION_H
