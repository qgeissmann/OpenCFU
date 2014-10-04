#ifndef GUI_IMGFILESETTER_H
#define GUI_IMGFILESETTER_H

#include "Gui_OptionSetterBaseClass.hpp"
#include "text.hpp"
#include <sstream>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/stock.h>
#include <glibmm/miscutils.h>
#include "defines.hpp"
#include <giomm/file.h>
#include "Gui_ConfigIO.hpp"

class Updatable{
    public:
        Updatable(std::vector<Glib::RefPtr<Gio::File> >& files,unsigned int& current_file_idx):
        m_files(files),
        m_current_file_idx(current_file_idx)
        {}

    virtual void update() = 0;
    protected:
        std::vector<Glib::RefPtr<Gio::File> >& m_files;
        unsigned int& m_current_file_idx;

};

class InfoLabel: public Updatable, public Gtk::Label{
    public:
        InfoLabel(std::vector<Glib::RefPtr<Gio::File> >& files,
           unsigned int& current_file_idx):
           Updatable(files,current_file_idx),
           m_no_img_txt(NO_FILE_LABEL){
           set_line_wrap();
//           set_line_wrap_mode(Pango::WRAP_WORD_CHAR );
           }

        void update(){
            std::stringstream ss;
            if(m_files.size()==0){
                ss<<m_no_img_txt;
            }

            else{
                ss  << "Image"
                    <<m_current_file_idx+1
                    <<"/"
                    <<m_files.size()
                    <<" ("
                    <<(m_files[m_current_file_idx])->get_basename()
                    <<")";
            }

            this->set_text(ss.str());
        }

    private:
        const std::string m_no_img_txt;
};


class FileBrowsingButton: public Updatable, public Gtk::Button{
    public:
        FileBrowsingButton(std::vector<Glib::RefPtr<Gio::File> >& files,
           unsigned int& current_file_idx, std::string str, int score):
           Updatable(files,current_file_idx), Gtk::Button(str), m_score(score){

               set_has_tooltip();
               switch(score){
                case -2:
                    set_tooltip_text(TOOLTIP_IMG_SETTER_FIRST);
                    break;
                case -1:
                    set_tooltip_text(TOOLTIP_IMG_SETTER_PREVIOUS);
                    break;
                case 1:
                    set_tooltip_text(TOOLTIP_IMG_SETTER_NEXT);
                    break;
                case 2:
                    set_tooltip_text(TOOLTIP_IMG_SETTER_LAST);
                    break;

               }
           }

        int getScore(){return m_score;}
        void update(){
            if(m_score < 0){
                if(m_current_file_idx == 0 || m_files.size() == 0 ){
                    this->set_sensitive(false);
                }
                else
                    this->set_sensitive();
            }

            else{
                if((m_current_file_idx == m_files.size() - 1) || m_files.size() == 0)
                    this->set_sensitive(false);
                else
                    this->set_sensitive();
            }
        }
    private:
        const int m_score;

};

class Gui_ImgFileSetter : public Gui_OptionSetterBaseClass{
    public:
        Gui_ImgFileSetter(Gui_ProcessorHandler& processor_hand,const std::string str, Gui_ConfigIO& config);
        void updateNFiles(int i){}
        sigc::signal<void,int> signal_publish_N_files(){ return m_signal_publish_N_files;}
        virtual ~Gui_ImgFileSetter();
    protected:
        bool updateOptions();
        void on_AddFileClicked();
        bool appendOrOverWrite(std::vector<std::string>&  selected_files);
        sigc::signal<void,int> signal_change_file(){ return m_signal_change_file;}
        void on_ChangeFile(int idx_mod);
        void updateAllWidgets();
        void recieveDragAndDrop(std::vector<std::string> strg);
        void publishNFiles(){ m_signal_publish_N_files.emit(m_files.size());}
        void on_realize();

//        void checkFiles( std::vector<Glib::RefPtr<Gio::File> >& files);
        void checkCorruptedFiles( std::vector<Glib::RefPtr<Gio::File> >& files);
        void checkRedondantFiles( std::vector<Glib::RefPtr<Gio::File> >& files);
    private:
        sigc::signal<void,int> m_signal_change_file;
        sigc::signal<void,int> m_signal_publish_N_files;
        Gtk::HBox m_hbox_butts;
        Gtk::Button m_add_file_button;
        std::vector<FileBrowsingButton*> m_file_buttons;
        const int m_button_scores[4];
        const std::string m_button_text[4];
        std::vector<Glib::RefPtr<Gio::File> > m_files;
        Glib::RefPtr<Gio::File> m_current_file;
        std::string& m_last_dir;
        unsigned int m_current_file_idx;
        Gtk::FileFilter m_img_filter,m_no_filter;
        InfoLabel m_label;
};

#endif // GUI_IMGFILESETTER_H
