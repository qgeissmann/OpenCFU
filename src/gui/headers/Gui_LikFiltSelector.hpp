#ifndef GUI_LIKFILTSELECTOR_H
#define GUI_LIKFILTSELECTOR_H

#include "Gui_OptionSetterBaseClass.hpp"
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/spinbutton.h>
#include "defines.hpp"
#include "text.hpp"

class Gui_LikFiltSelector: public Gui_OptionSetterBaseClass
{
    public:
        Gui_LikFiltSelector(Gui_ProcessorHandler& processor_hand,const std::string str);
        void updateNFiles(int i);
    protected:
        bool updateOptions();

        void on_realize(){
            this->on_tick_box();
            Gui_OptionSetterBaseClass::on_realize();}

        void on_tick_box(){ m_check_butt.get_active() ?  m_hbox2.show() : m_hbox2.hide(); }

        void on_activate_filter(){
            this->on_tick_box();
            this->setOption();}
    private:
        Gtk::HBox m_hbox2;
        Gtk::Adjustment m_adjust_thr;
        Gtk::SpinButton m_spin_butt_thr;
        Gtk::CheckButton m_check_butt;
        Gtk::Label m_lab_thr;
};

#endif // GUI_LIKFILTSELECTOR_H
