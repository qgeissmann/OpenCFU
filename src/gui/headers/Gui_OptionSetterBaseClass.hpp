#ifndef GUI_OPTIONSETTERBASECLASS_H
#define GUI_OPTIONSETTERBASECLASS_H

#include <gtkmm/box.h>
#include <gtkmm/frame.h>
#include <gtkmm/bin.h>
//#include <gtkmm/separator.h>
#include <gtkmm/label.h>
#include <sigc++/sigc++.h>
#include "ProcessingOptions.hpp"
#include "Gui_ProcessorHandler.hpp"


class Gui_OptionSetterBaseClass: public Gtk::Frame
{
    public:
        Gui_OptionSetterBaseClass(Gui_ProcessorHandler& processor_hand,const std::string name);



        bool setOption();
        virtual void updateNFiles(int i) = 0;
        virtual ~Gui_OptionSetterBaseClass();
    protected:
        bool lockMutexAndUpdate();
        void on_realize(){updateOptions();Gtk::Frame::on_realize();}
        virtual bool updateOptions() = 0;
        Gtk::VBox m_vbox;
        Gui_ProcessorHandler& m_processor_hand;
        ProcessingOptions& m_opts;
        sigc::signal<bool> m_signal_change_options;
//    private:






};


#endif // GUI_OPTIONSETTERBASECLASS_H
