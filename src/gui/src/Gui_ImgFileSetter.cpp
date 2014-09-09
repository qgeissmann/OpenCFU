#include "Gui_ImgFileSetter.hpp"
#include "Gui_MyWindow.hpp"
#include "opencv2/imgproc/imgproc.hpp"


Gui_ImgFileSetter::Gui_ImgFileSetter(Gui_ProcessorHandler& processor_hand,const std::string str, Gui_ConfigIO& config):
    Gui_OptionSetterBaseClass(processor_hand,str),
    m_add_file_button(ADD_FILE_BUTTON_TEXT),
    m_file_buttons(4),
    m_button_scores{-2,-1,1,2},
    m_button_text{"<<","<",">",">>"},
    m_last_dir(config.getLastDir()),
    m_current_file_idx(0),
    m_label(m_files,m_current_file_idx)
{
    set_tooltip_text(TOOLTIP_IMG_SETTER);
    this->m_vbox.add(m_label);
    this->m_vbox.add(m_add_file_button);
    this->m_vbox.add(m_hbox_butts);

    for(unsigned int i = 0;i<4;i++){
        FileBrowsingButton *fb  = Gtk::manage(new FileBrowsingButton(m_files,m_current_file_idx,m_button_text[i],m_button_scores[i]));
        m_file_buttons[i] = fb;
        m_hbox_butts.add(*fb);
        fb->signal_clicked().connect( sigc::bind(sigc::mem_fun(m_signal_change_file,&sigc::signal<void, int>::emit),fb->getScore()));
    }

    m_add_file_button.signal_clicked().connect( sigc::mem_fun(*this,&Gui_ImgFileSetter::on_AddFileClicked) );
    signal_change_file().connect( sigc::mem_fun(*this,&Gui_ImgFileSetter::on_ChangeFile) );
    signal_change_file().connect(
            sigc::hide(sigc::mem_fun(*this,&Gui_ImgFileSetter::publishNFiles))
         );

    m_files.reserve(100);
    m_img_filter.set_name("Images");
    m_img_filter.add_mime_type("image/*");
    m_img_filter.add_pattern("*.jpeg");
    m_img_filter.add_pattern("*.jpg");
    m_img_filter.add_pattern("*.JPG");
    m_img_filter.add_pattern("*.JPEG");
    m_img_filter.add_pattern("*.TIF");
    m_img_filter.add_pattern("*.TIFF");
    m_img_filter.add_pattern("*.tif");
    m_img_filter.add_pattern("*.tiff");
    m_img_filter.add_pattern("*.BMP");
    m_img_filter.add_pattern("*.bmp");
    m_img_filter.add_pattern("*.png");
    m_img_filter.add_pattern("*.PNG");
    m_img_filter.add_pattern("*.gif");
    m_img_filter.add_pattern("*.GIF");

    m_no_filter.set_name("Any files");
    m_no_filter.add_pattern("*");
    this->updateAllWidgets();

}

bool Gui_ImgFileSetter::updateOptions(){
    DEV_INFOS("Trying to send a processing option");
    std::string tmp_path = m_current_file->get_path();
    m_last_dir = Glib::path_get_dirname(m_current_file->get_path());
    return m_opts.setImage(tmp_path);
}

Gui_ImgFileSetter::~Gui_ImgFileSetter(){
}

void Gui_ImgFileSetter::on_realize (){
    ((Gui_MyWindow*)get_toplevel())->signal_drag_and_drop().connect(sigc::mem_fun(*this,&Gui_ImgFileSetter::recieveDragAndDrop));
    publishNFiles();
    Gtk::Frame::on_realize();
}


void Gui_ImgFileSetter::on_ChangeFile(int idx_mod){

    if(m_files.size()>0){
        if(idx_mod == -1 && m_current_file_idx>0){
            m_current_file_idx += idx_mod;
        }

        else if(idx_mod == 1 && m_current_file_idx < (m_files.size()-1)){
            m_current_file_idx += idx_mod;
        }


        else if (idx_mod == -2 && m_current_file_idx>0){
            m_current_file_idx = 0;
        }

        else if(idx_mod == 2 && m_current_file_idx < (m_files.size()-1)){
            m_current_file_idx = (m_files.size()-1);
        }
        m_current_file = m_files[m_current_file_idx];
        m_signal_change_options.emit();
    }
    else{
        DEV_INFOS("Not Changing file: no files");
    }
    this->updateAllWidgets();
}

void Gui_ImgFileSetter::updateAllWidgets(){
    m_label.update();
    for(unsigned int i = 0;i<m_file_buttons.size();i++){
            (m_file_buttons[i])->update();
    }
}

void Gui_ImgFileSetter::on_AddFileClicked(){
    Gtk::FileChooserDialog dialog("Please choose a file",Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

    dialog.add_filter(m_img_filter);
    dialog.add_filter(m_no_filter);

    dialog.set_select_multiple(true);
    dialog.set_current_folder(m_last_dir);
    dialog.set_transient_for(*((Gtk::Window*) this->get_toplevel()));
    bool success = false;
    while(!success){
        int result = dialog.run();
        switch(result){
            case Gtk::RESPONSE_OK:
                {
                std::vector<std::string>  selected_files = dialog.get_uris();
                dialog.hide();
                success = this->appendOrOverWrite(selected_files);
                dialog.show();
                break;
                }
            case Gtk::RESPONSE_CANCEL:
                {
                success = true;
                break;
                }
            default:
                {
                success = true;
                break;
                }
        }
    }
}

void Gui_ImgFileSetter::checkRedondantFiles( std::vector<Glib::RefPtr<Gio::File> >& files){
    std::vector<Glib::RefPtr<Gio::File> > new_files;
    new_files.reserve(files.size());
    for(auto& f:files){
        bool exist(false);
        for(auto& mf:m_files)
            if(mf->equal(f))
                exist = true;
        if(!exist)
            new_files.push_back(f);
    }
    std::swap(new_files,files);
}
//
void Gui_ImgFileSetter::checkCorruptedFiles( std::vector<Glib::RefPtr<Gio::File> >& files){
    std::vector<Glib::RefPtr<Gio::File> > new_files;
    new_files.reserve(files.size());

    for(auto& f:files){
        std::string str= f->get_path();
        if(!cv::imread(str).empty())
            new_files.push_back(f);
    }
    std::swap(new_files,files);
}

bool Gui_ImgFileSetter::appendOrOverWrite(std::vector<std::string>&  selected_files){
    int result;
    std::vector<Glib::RefPtr<Gio::File> > files;

    for(auto& str : selected_files){
        files.push_back(Gio::File::create_for_uri(str));
    }

    this->checkCorruptedFiles(files);

    if(files.empty()){
        Gtk::MessageDialog dial(NO_FILES, false, Gtk::MESSAGE_ERROR,  Gtk::BUTTONS_OK );
        dial.set_secondary_text(FILE_CORRUPTED_SECONDARY);
        dial.set_transient_for(*((Gtk::Window*) this->get_toplevel()));
        dial.run();
        return false;
    }
    else if (files.size() < selected_files.size()){
        std::stringstream ss;
        ss<<selected_files.size() - files.size()<<FILE_REMOVED;


        Gtk::MessageDialog dial(ss.str(), false, Gtk::MESSAGE_WARNING,  Gtk::BUTTONS_OK );
        dial.set_secondary_text(FILE_CORRUPTED_SECONDARY);
        dial.set_transient_for(*((Gtk::Window*) this->get_toplevel()));
        dial.run();
    }

    if(m_files.size()>0){
        Gtk::MessageDialog dial(APPEND_OR_OVERWRITE_MESSAGE, false, Gtk::MESSAGE_QUESTION,  Gtk::BUTTONS_NONE );
        dial.set_secondary_text(APPEND_OR_OVERWRITE_MESSAGE2);

        dial.add_button("Append",Gtk::RESPONSE_NO);
        dial.add_button("Overwrite",Gtk::RESPONSE_YES);
        dial.add_button(Gtk::Stock::CANCEL,Gtk::RESPONSE_CANCEL);
        dial.set_transient_for(*((Gtk::Window*) this->get_toplevel()));
        result = dial.run();
    }
    else
        result = Gtk::RESPONSE_YES;



    if(result == Gtk::RESPONSE_YES){
        m_files = files;
        this->m_signal_change_file.emit(-2);
        return true;
    }
    else if(result == Gtk::RESPONSE_NO){
        unsigned int siz = files.size();
        checkRedondantFiles(files);
        if(files.size() == 0){
            Gtk::MessageDialog dial(NO_FILES, false, Gtk::MESSAGE_ERROR,  Gtk::BUTTONS_OK );
            dial.set_secondary_text(FILE_REDONDANT_SECONDARY);
            dial.set_transient_for(*((Gtk::Window*) this->get_toplevel()));
            dial.run();
            return false;

        }
        else{
            if(files.size() < siz){

                std::stringstream ss;
                ss<<siz- files.size()<<FILE_REMOVED;
                Gtk::MessageDialog dial(ss.str(), false, Gtk::MESSAGE_WARNING,  Gtk::BUTTONS_OK );
                dial.set_secondary_text(FILE_REDONDANT_SECONDARY);
                dial.set_transient_for(*((Gtk::Window*) this->get_toplevel()));
                dial.run();
            }
            m_files.insert( m_files.end(), files.begin(), files.end() );
            this->m_signal_change_file.emit(0);
            return true;
        }

    }
    else
        return false;
}


void Gui_ImgFileSetter::recieveDragAndDrop(std::vector<std::string> strg){
    DEV_INFOS("drag and drop");
    appendOrOverWrite(strg);
}
