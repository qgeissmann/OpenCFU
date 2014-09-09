#ifndef GUI_CONFIGIO_H
#define GUI_CONFIGIO_H

#include "defines.hpp"
#include <glibmm/keyfile.h>
#include <giomm/file.h>
#include "ProcessingOptions.hpp"

class Gui_ConfigIO
{
    public:
        Gui_ConfigIO(ProcessingOptions& opts);
        ~Gui_ConfigIO();
        int& getHelloLevelRef(){return m_hello_lev;}
        std::string& getLastDir(){
            return m_last_working_dir;}
    protected:
        void makeNewConfig();
        void readConfig();
        void getOptionsFromConf();
    private:
        int m_hello_lev;
        std::string m_config_file_name;
        std::string m_version;
        ProcessingOptions& m_opts;
        Glib::KeyFile m_key_file;
        Glib::RefPtr<Gio::File> m_file;
        std::string m_last_working_dir;

};

#endif // GUI_CONFIGIO_H
