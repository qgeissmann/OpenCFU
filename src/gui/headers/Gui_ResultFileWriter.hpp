#ifndef GUI_RESULTFILEWRITER_H
#define GUI_RESULTFILEWRITER_H
#include "ResultMap.hpp"
#include "text.hpp"
#include <sstream>
#include <giomm/file.h>
#include <gtkmm/dialog.h>
#include <gtkmm/stock.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/filechooserdialog.h>


class SaveFileChooser : public Gtk::FileChooserDialog{
    public:
        SaveFileChooser(std::string last_uri):
            Gtk::FileChooserDialog(SAVE_FILE_TITLE, Gtk:: FILE_CHOOSER_ACTION_SAVE){
                DEV_INFOS(last_uri);
                if(!last_uri.empty())
                    set_uri(last_uri);
                else
                    set_uri(Glib::build_filename("file://" ,Glib::get_home_dir()));


                set_select_multiple(false);
                set_do_overwrite_confirmation(true);
                this->set_create_folders(true);
                this->add_button(Gtk::Stock::OK,Gtk::RESPONSE_OK);
                this->add_button(Gtk::Stock::CANCEL,Gtk::RESPONSE_CANCEL);
                this->set_current_name("my_results.csv");

                Gtk::FileFilter filter, no_filter;

                filter.set_name("CSV Files");
                filter.add_mime_type("text/csv");
                filter.add_pattern("*.csv");
                no_filter.set_name("Any files");
                no_filter.add_pattern("*");

                this->add_filter(filter);
                this->add_filter(no_filter);
                }
};


class NoSelectionMessage : public Gtk::MessageDialog{
    public:
        NoSelectionMessage():
            Gtk::MessageDialog(NO_SELECTION_MESSAGE,true, Gtk::MESSAGE_ERROR,Gtk::BUTTONS_OK){}
};

class SaveModeMessage : public Gtk::MessageDialog{
    public:
        SaveModeMessage():
            Gtk::MessageDialog(SAVE_MODE_MESSAGE,true, Gtk::MESSAGE_QUESTION,Gtk::BUTTONS_NONE){
            this->add_button("Summary",Gtk::RESPONSE_ACCEPT);
            this->add_button("Detailled",Gtk::RESPONSE_OK);
            this->add_button(Gtk::Stock::CANCEL,Gtk::RESPONSE_CANCEL);
        }
};


class Gui_ResultFileWriter
{
    public:
        Gui_ResultFileWriter(Gtk::Widget* parent);
        bool saveSelection(const ResultMap& res_map, const std::vector<int>& idxs);
        void setInUpToDate(bool up_to_date){m_up_to_date = up_to_date;}
        bool getInUpToDate(){return m_up_to_date;}
    protected:
         void writeHeader(const bool detail,const ResultMap& res_map, const std::vector<int>& idxs);
         void writeRows(const bool detail,const ResultMap& res_map,const std::vector<int>& idxs);

    private:
        Gtk::Widget* m_parent_widg;
        bool m_up_to_date;

        Glib::RefPtr<Gio::File> m_out_file;

};

#endif // GUI_RESULTFILEWRITER_H
