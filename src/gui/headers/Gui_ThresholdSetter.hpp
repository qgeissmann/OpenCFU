#ifndef GUI_THRESHOLDSETTER_H
#define GUI_THRESHOLDSETTER_H

#include "defines.hpp"
#include "Gui_OptionSetterBaseClass.hpp"
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/comboboxtext.h>
#include "text.hpp"


class Gui_ThresholdSetter:public Gui_OptionSetterBaseClass
{
    public:
        Gui_ThresholdSetter(Gui_ProcessorHandler& processor_hand,const std::string str);
        void updateNFiles(int i);
    protected:
        bool updateOptions();
    private:
        Gtk::HBox m_hbox2;
        Gtk::Adjustment m_adjust;
        Gtk::SpinButton m_spin_butt;
        Gtk::CheckButton m_check_butt;
        Gtk::ComboBoxText m_combo;
        std::map<std::string,int> m_combo_map;
//        bool dummy_variable;
};

#endif // GUI_THRESHOLDSETTER_H
