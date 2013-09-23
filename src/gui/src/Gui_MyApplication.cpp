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
//        std::stringstream ss;
//        ss<<INSTALLDIR<<"/";
//        prefix = ss.str();
    #endif



//    std::vector<std::string> image_path;
//    image_path.push_back(ICON128_IMG);
//    image_path.push_back(ICON64_IMG);
//    image_path.push_back(ICON48_IMG);
//    image_path.push_back(ICON24_IMG);
//    for(auto& i : image_path){
//
//        std::stringstream ss;
//        ss<<prefix<<i;
//        if (!g_file_test(ss.str().c_str(),G_FILE_TEST_EXISTS)){
//            ss.str(std::string());
//            ss <<"./"<<i;
//            DEV_INFOS("Cannot open icon! Trying to open @ "<<ss.str().c_str()<<" instead.");
//            if(!g_file_test(ss.str().c_str(),G_FILE_TEST_EXISTS));
//                DEV_INFOS("No icon file! check installation");
//        }
//        m_icon_list.push_back(Gdk::Pixbuf::create_from_file(ss.str()));
//
//
//    }


//    Gtk::Window::set_default_icon_list(m_icon_list);


    if(m_config.getHelloLevelRef() <2){
        Gui_HelloWindow hello_window(m_config.getHelloLevelRef());
        Gtk::Main::run(hello_window);
    }

    Gui_MyWindow my_window(m_pro_hand,m_opts);
    my_window.maximize();
    Gtk::Main::run(my_window);
}
