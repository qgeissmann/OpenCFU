#ifndef GUI_CONTROLPANEL_H
#define GUI_CONTROLPANEL_H

#include <gtkmm/box.h>
#include <gtkmm/frame.h>
#include <gtkmm/aboutdialog.h>

#include "Gui_ProcessorHandler.hpp"
#include "Gui_ResultLabel.hpp"
#include "Gui_ImgFileSetter.hpp"
#include "Gui_ThresholdSetter.hpp"
#include "Gui_RadiusSetter.hpp"
#include "Gui_ColourSelector.hpp"
#include "Gui_LikFiltSelector.hpp"
#include "Gui_HelloWindow.hpp"
#include "Gui_PixbufOpener.hpp"
#include "Gui_MaskSetter.hpp"
#include "Gui_ColourCluster.hpp" //NJL 10/AUG/2014
#include "Gui_ConfigIO.hpp"

class MyAboutDialog : public Gtk::AboutDialog
{
    public:
        MyAboutDialog(): Gtk::AboutDialog(),m_link(PACKAGE_URL,PACKAGE_URL){
            set_version(PACKAGE_VERSION);
            std::vector<Glib::ustring> authors;
            authors.push_back("Quentin Geissmann<opencfu@gmail.com>");
            set_authors(authors);
            set_license("GPL_3_0");
            set_copyright("Quentin Geissmann 2012-2013");

            #if defined _WIN64 || defined _WIN32

            std::stringstream ss;
            ss<<ABOUT_TEXT<<std::endl<<PACKAGE_URL;
            set_comments(ss.str());
            #else
            set_website(PACKAGE_URL);
            set_website_label("Website");
            set_comments(ABOUT_TEXT);
            #endif

        set_logo(Gui_PixbufOpener::pixbufOpen(LOGO_IMG));

        }
    protected:
        void on_activate_link(const std::string& str){
            DEV_INFOS(str);
        }
    private:
        PortableLinkButton m_link;
};

class Gui_ControlPanel: public Gtk::Frame
{
    public:
        Gui_ControlPanel(Gui_ProcessorHandler& processor_hand,ResultMap& result_map, Gui_ConfigIO& config);

        sigc::signal<void,bool>& signal_set_to_NA(){
            return m_result_label.signal_set_to_NA();}

        sigc::signal<void,bool,double>& signal_show_decorator(){
            return m_result_label.signal_show_decorator();}
        sigc::signal<void,bool> signal_show_mask(){return m_mask_setter.signal_show_mask();}
        void on_about_button();


    private:
        Gui_ProcessorHandler& m_processor_hand;
        Gui_ResultLabel m_result_label;
        Gui_ImgFileSetter m_file_setter;
        Gui_ThresholdSetter m_thr_setter;
        Gui_RadiusSetter m_rad_setter;
        Gui_MaskSetter m_mask_setter;
        Gui_ColourSelector m_col_select;
        Gui_LikFiltSelector m_lik_select;
        Gui_ColourCluster m_col_cluster; //NJL 10/AUG/2014
        Gtk::VBox m_vbox;
        Gtk::Button m_about_opencfu_button;

};

#endif // GUI_CONTROLPANEL_H
