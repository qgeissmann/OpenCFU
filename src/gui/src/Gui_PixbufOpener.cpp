#include "Gui_PixbufOpener.hpp"


 Glib::RefPtr<Gdk:: Pixbuf > Gui_PixbufOpener::pixbufOpen(const std::string& name){
    GFileTest test =  G_FILE_TEST_EXISTS;
        std::string path = "./"+name;
        DEV_INFOS("TRYING TO OPEN LOCALY "<<name<<" at "<<path);
        if(!g_file_test(path.c_str(),test)){
            DEV_INFOS("NO LOCAL FILE -> falling back on installed files");
                std::string prefix;
                #if defined _WIN64 || defined _WIN32
                prefix = "..";
                #else
                prefix = INSTALLDIR;
                #endif
                path = prefix+"/"+name;

                DEV_INFOS("TRYING TO OPEN LOCALY "<<name<<" at "<<path);

                if(!g_file_test(path.c_str(),test)){
                    DEV_INFOS("NO INSTALLED FILE");
                    //todo exeption

                }
        }
               Glib::RefPtr<Gdk:: Pixbuf >  pb = Gdk::Pixbuf::create_from_file(path);
        //todo check
    return pb;
 }
