#include "Gui_OptionSetterBaseClass.hpp"

Gui_OptionSetterBaseClass::Gui_OptionSetterBaseClass(Gui_ProcessorHandler& processor_hand, const std::string name):
    Gtk::Frame(name),
    m_processor_hand(processor_hand),
    m_opts(m_processor_hand.getOptions())
{

    this->add(m_vbox);
    this->set_border_width(3);
    Gtk::Label* l;
    l = (Gtk::Label*) this->get_label_widget();
    l->set_use_markup();
    m_signal_change_options.connect(sigc::mem_fun(*this,&Gui_OptionSetterBaseClass::setOption));
    set_has_tooltip();
}

Gui_OptionSetterBaseClass::~Gui_OptionSetterBaseClass()
{
}

bool Gui_OptionSetterBaseClass::setOption(){
    DEV_INFOS("Setting an option: " + std::string(typeid(*this).name()));
    bool result = lockMutexAndUpdate();
    if(!result){
        DEV_INFOS("did not work !");
    }
    else{
         m_processor_hand.onRunProcess();
    }
    return result;

}

bool Gui_OptionSetterBaseClass::lockMutexAndUpdate(){
    Glib::Mutex::Lock lock (m_processor_hand.getMutex());
    return this->updateOptions();
}
