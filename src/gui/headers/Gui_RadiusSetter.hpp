#ifndef GUI_RADIUSSETTER_H
#define GUI_RADIUSSETTER_H


#include "Gui_OptionSetterBaseClass.hpp"
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/spinbutton.h>
#include "defines.hpp"
#include "text.hpp"


class Gui_RadiusSetter:public Gui_OptionSetterBaseClass
{
    public:
        Gui_RadiusSetter(Gui_ProcessorHandler& processor_hand,const std::string str);
        void updateNFiles(int i);
    protected:
        bool updateOptions();
        void on_MinRadChanged(){
                int val_min = m_spin_butt_min.get_value();
                int val_max = m_spin_butt_max.get_value();
                if(val_min > val_max)
                    m_spin_butt_max.set_value(val_min);
                else
                    this->setOption();
        }
    private:
        Gtk::HBox m_hbox2;
        Gtk::Adjustment m_adjust_min,m_adjust_max;
        Gtk::SpinButton m_spin_butt_min, m_spin_butt_max;
        Gtk::CheckButton m_check_butt;
        Gtk::Label m_lab_min, m_lab_max;
};



#endif // GUI_RADIUSSETTER_H
