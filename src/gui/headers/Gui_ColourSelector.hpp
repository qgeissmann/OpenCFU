#ifndef GUI_COLOURSELECTOR_H
#define GUI_COLOURSELECTOR_H

#include "Gui_OptionSetterBaseClass.hpp"
#include "Gui_ColourWheel.hpp"
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/spinbutton.h>
#include "defines.hpp"
#include "text.hpp"

class Gui_ColourSelector : public Gui_OptionSetterBaseClass
{
    public:
        Gui_ColourSelector(Gui_ProcessorHandler& processor_hand,const std::string str);
        void updateNFiles(int i);


    protected:
        void on_MinSatChanged(){
                int val_min = m_spin_butt_sat_min.get_value();
                int val_max = m_spin_butt_sat_max.get_value();
                if(val_min > val_max)
                    m_spin_butt_sat_max.set_value(val_min);
                else
                    this->setOption();
        }
        bool setOption(){
            m_col_wheel.queue_draw();
            return Gui_OptionSetterBaseClass::setOption();
        }
        bool updateOptions();

        void on_realize(){
            this->on_tick_box();
            Gui_OptionSetterBaseClass::on_realize();}

        void on_tick_box(){ m_check_butt.get_active() ?  m_hbox2.show() : m_hbox2.hide(); }

        void on_activate_filter(){
            this->on_tick_box();
            this->setOption();}


    private:
        Gtk::HBox m_hbox2,m_hbox21,m_hbox22;
        Gtk::VBox m_vbox2;
        Gtk::Adjustment m_adjust_hue_center,m_adjust_hue_tol;
        Gtk::Adjustment m_adjust_sat_min,m_adjust_sat_max;
        Gtk::SpinButton m_spin_butt_hue_center, m_spin_butt_hue_tol;
        Gtk::SpinButton m_spin_butt_sat_min, m_spin_butt_sat_max;
        Gtk::CheckButton m_check_butt;
        Gtk::Label m_lab_hue, m_lab_center, m_lab_tol;

        Gtk::Label m_lab_sat, m_lab_sat_min, m_lab_sat_max;

        Gui_ColourWheel m_col_wheel;
};

#endif // GUI_COLOURSELECTOR_H
