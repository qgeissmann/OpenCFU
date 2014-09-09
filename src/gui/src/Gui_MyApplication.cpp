#include "Gui_MyApplication.hpp"

Gui_MyApplication::Gui_MyApplication(int argc,char** argv,Processor& pro):
Gtk::Main(argc,argv),
m_processor(pro),
m_pro_hand(m_processor,m_opts),
m_config(m_opts)
{


    DEV_INFOS("Building GUI");

    std::string prefix;
    #if defined _WIN64 || _WIN32
        Gtk::Window::set_default_icon_list(m_icon_list);
        m_icon_list.push_back(Gui_PixbufOpener::pixbufOpen(ICON128_IMG));
        m_icon_list.push_back(Gui_PixbufOpener::pixbufOpen(ICON64_IMG));
        m_icon_list.push_back(Gui_PixbufOpener::pixbufOpen(ICON48_IMG));
        m_icon_list.push_back(Gui_PixbufOpener::pixbufOpen(ICON24_IMG));
        Gtk::Window::set_default_icon_list(m_icon_list);
    #else
        Gtk::Window::set_default_icon_name 	("opencfu");
    #endif


    if(m_config.getHelloLevelRef() <2){
        Gui_HelloWindow hello_window(m_config.getHelloLevelRef());
        Gtk::Main::run(hello_window);
    }

    Gui_MyWindow my_window(m_pro_hand,m_config);
    my_window.maximize();
    Gtk::Main::run(my_window);
}
