#include "Gui_ConfigIO.hpp"
#include <glibmm.h>



Gui_ConfigIO::Gui_ConfigIO(ProcessingOptions& opts):
m_hello_lev(0),
m_opts(opts)
{
    readConfig();
}

Gui_ConfigIO::~Gui_ConfigIO(){
    makeNewConfig();
}
void Gui_ConfigIO::makeNewConfig(){

    m_key_file.set_integer("GUI","hello_level",m_hello_lev);
    m_key_file.set_string("GUI","last_wd", m_last_working_dir);
    m_key_file.set_boolean("Processing","Has_auto-threshold",m_opts.getHasAutoThr());
    m_key_file.set_boolean("Processing","Has_max-radius",m_opts.getHasMaxRad());
    m_key_file.set_boolean("Processing","Has_hue-filter",m_opts.getHasHueFilt());
    m_key_file.set_boolean("Processing","Has_outlier-filter",m_opts.getHasOutlierFilt());

    m_key_file.set_integer("Processing","Threshold",m_opts.getThr());
    m_key_file.set_integer("Processing","Threshold-mode",m_opts.getThrMode());

    m_key_file.set_integer("Processing","Min-rad",m_opts.getMinMaxRad().first);
    m_key_file.set_integer("Processing","Max-rad",m_opts.getMinMaxRad().second);

    m_key_file.set_integer("Processing","Hue-center",m_opts.getCenTolHue().first);
    m_key_file.set_integer("Processing","Hue-tolerance",m_opts.getCenTolHue().second);

    m_key_file.set_integer("Processing","Satur-min-saturation",m_opts.getMinMaxSat().first);
    m_key_file.set_integer("Processing","Satur-max-saturation",m_opts.getMinMaxSat().second);

    m_key_file.set_double("Processing","Likelihood-threshold",m_opts.getLikeThr());

    m_key_file.set_double("Processing","Clustering-Distance",m_opts.getClustDist());

    m_key_file.set_string("General","former_version",PACKAGE_VERSION);


    m_file = Gio::File::create_for_path(m_config_file_name.c_str());
    Glib::RefPtr< Gio::FileOutputStream > fout = m_file->replace();
    fout->write(m_key_file.to_data());
    fout->close();
}


void Gui_ConfigIO::getOptionsFromConf(){
    m_hello_lev =  m_key_file.get_integer("GUI","hello_level");//,0);

    m_opts.setHasAutoThr(m_key_file.get_boolean("Processing","Has_auto-threshold"));

    m_opts.setHasMaxRad(m_key_file.get_boolean("Processing","Has_max-radius"));

    m_opts.setHasHueFilt(m_key_file.get_boolean("Processing","Has_hue-filter"));

    m_opts.setHasOutlierFilt(m_key_file.get_boolean("Processing","Has_outlier-filter"));


    m_opts.setThr(m_key_file.get_integer("Processing","Threshold"));

    m_opts.setThrMode(m_key_file.get_integer("Processing","Threshold-mode"));


    m_opts.setMinMaxRad(std::pair<int,int>(m_key_file.get_integer("Processing","Min-rad"),m_key_file.get_integer("Processing","Max-rad")));

    m_opts.setCenTolHue(std::pair<int,int>(m_key_file.get_integer("Processing","Hue-center"),m_key_file.get_integer("Processing","Hue-tolerance")));

    m_opts.setCenTolHue(std::pair<int,int>(m_key_file.get_integer("Processing","Satur-min-saturation"),m_key_file.get_integer("Processing","Satur-max-saturation")));

    m_opts.setLikeThr(m_key_file.get_double("Processing","Likelihood-threshold"));

    //This exception block catches the instance where an older config file is used,
    //In some cases, on the first run, the parameter "Clustering-Distance" will not be defined.
    try {
        m_opts.setClustDist(m_key_file.get_double("Processing","Clustering-Distance"));
    }
    catch (Glib::KeyFileError) {
        m_opts.setClustDist(2.3);
    }

    try {
        m_version = m_key_file.get_string("General","former_version");
    }
    catch (Glib::KeyFileError) {
    m_version = "3.8.0";
    }

    try{

        m_last_working_dir =  m_key_file.get_string("GUI","last_wd");
        if (!g_file_test(m_last_working_dir.c_str(),G_FILE_TEST_EXISTS))
            throw std::invalid_argument("the working directory does not exist");
    }
    catch(...){
        m_last_working_dir = Glib::get_home_dir();
    }
}


void Gui_ConfigIO::readConfig(){
//    std::string config_dir_name = Glib::build_filename(Glib::get_user_config_dir(), Glib::get_application_name());
    std::string config_dir_name = Glib::build_filename(Glib::get_user_config_dir(), std::string(PACKAGE_TARNAME));
    DEV_INFOS("configuration directory should be:"<<config_dir_name);
    if(g_file_test(config_dir_name.c_str(),G_FILE_TEST_IS_DIR)){
        DEV_INFOS("Directory exist already");
    }
    else{
        DEV_INFOS("Creating the directory");
        Gio::File::create_for_path(config_dir_name.c_str())->make_directory_with_parents();
    }

    std::stringstream ss;
    ss << Glib::get_prgname()<<".conf";
    m_config_file_name = Glib::build_filename(config_dir_name,ss.str());


    if(g_file_test(m_config_file_name.c_str(),G_FILE_TEST_EXISTS)){
        DEV_INFOS(m_config_file_name);

        assert(m_key_file.load_from_file(m_config_file_name,Glib::KEY_FILE_NONE));
        this->getOptionsFromConf();
        if(m_version != PACKAGE_VERSION){
            this->makeNewConfig();
            m_hello_lev = 0;
        }
    }
    else{
        this->makeNewConfig();
    }


}
