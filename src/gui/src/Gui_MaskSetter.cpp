#include "Gui_MaskSetter.hpp"
#include "opencv2/imgproc/imgproc.hpp"
//#include "Gui_MyWindow.hpp"

#include "Gui_Display.hpp"

Gui_MaskSetter::Gui_MaskSetter(Gui_ProcessorHandler& processor_hand,const std::string str):
    Gui_OptionSetterBaseClass(processor_hand,str),m_last_dir(Glib::get_home_dir()),
    m_remake_button(Gtk::Stock::REFRESH), m_check_butt("Show")
{
    //todo
    set_tooltip_text(TOOLTIP_MASK_SETTER);
    m_check_butt.set_tooltip_text(TOOLTIP_MASK_SETTER_SHOW);
    m_remake_button.set_tooltip_text(TOOLTIP_MASK_SETTER_REFRESH);

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


    m_vbox.pack_start( m_combo);

    m_combo.append(MASK_TYPE_NONE,"None");
    m_combo.append(MASK_TYPE_DRAW,"Draw mask");
    m_combo.append(MASK_TYPE_FILE,"From file");
    m_combo.append(MASK_TYPE_AUTO,"Auto");



    this->m_vbox.pack_start(m_hbox);
    m_hbox.pack_start(m_check_butt);
    m_hbox.pack_start(m_remake_button);

    m_check_butt.set_active();
    m_check_butt.set_sensitive(false);
    m_remake_button.set_sensitive(false);

    m_combo.signal_select_type().connect(sigc::mem_fun(*this,&Gui_MaskSetter::on_combo_changed));
    m_combo.signal_change_state().connect(sigc::mem_fun(*this,&Gui_MaskSetter::on_update_hbox));
    m_remake_button.signal_clicked().connect(sigc::mem_fun(*this,&Gui_MaskSetter::on_remake_mask));
    m_check_butt.signal_clicked().connect(sigc::mem_fun(*this,&Gui_MaskSetter::on_show_mask));
    m_combo.set_title("ROI type");


}

void Gui_MaskSetter::on_remake_mask(){
    on_combo_changed(m_combo.getState());
}

void Gui_MaskSetter::on_update_hbox(int state){
    if(state == MASK_TYPE_NONE || state == MASK_TYPE_AUTO)
        m_remake_button.set_sensitive(false);
    else
        m_remake_button.set_sensitive(true);

    if(state == MASK_TYPE_NONE)
        m_check_butt.set_sensitive(false);
    else
        m_check_butt.set_sensitive(true);
}

bool Gui_MaskSetter::on_combo_changed(int state){
    DEV_INFOS("change combo");
    DEV_INFOS(state);

    if(state == MASK_TYPE_NONE){
        DEV_INFOS("Noning mask");
        m_mask.none();
        this->setOption();
        return true;
    }
    else if(state == MASK_TYPE_DRAW){
        if(drawMask()){
            this->setOption();
            return true;
        }
    }
    else if(state == MASK_TYPE_AUTO){
        m_mask.type = MASK_TYPE_AUTO;
        this->setOption();
        return true;
    }

    else if(state == MASK_TYPE_FILE){
        if(on_AddFileClicked()){
            this->setOption();
            return true;
        }
    }

    return false;
}



bool Gui_MaskSetter::updateOptions(){
    DEV_INFOS("Trying to send a processing option");
    return m_opts.setMask(m_mask);
}

bool Gui_MaskSetter::on_AddFileClicked(){
    Gtk::FileChooserDialog dialog("Please choose a mask file",Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

    dialog.add_filter(m_img_filter);
    dialog.add_filter(m_no_filter);

    dialog.set_select_multiple(false);
    dialog.set_current_folder(m_last_dir);
    dialog.set_transient_for(*((Gtk::Window*) this->get_toplevel()));

    bool success = false;
    while(!success){
        int result = dialog.run();
        switch(result){
            case Gtk::RESPONSE_OK:
                {
                Glib::RefPtr<Gio::File> file = Gio::File::create_for_uri(dialog.get_uri());
                dialog.hide();
                success = checkFile(file);
                dialog.show();

                if(success){
                    m_file = file;
                    MaskROI(m_file->get_path()).copyTo(m_mask);
                    return true;
                }

                break;
                }
            case Gtk::RESPONSE_CANCEL:
                {
                return false;
                break;
                }
            default:
                {
                return false;
                break;
                }
        }
    }
    return false;
}

bool Gui_MaskSetter::checkFile( const Glib::RefPtr<Gio::File>& file){
    std::string str= file->get_path();
    cv::Mat mat = cv::imread(str);
    cv::Mat tmp_mat;
    bool has_mask =  m_processor_hand.getMaskAsRGB(tmp_mat);
    if(mat.empty()){
        DEV_INFOS("corrupted file");
        return false;
    }
    else if(mat.channels() == 1){
        DEV_INFOS("convertion to greyscale");
        return true;
    }
    else if(!has_mask){
        return true;
    }
    else if(mat.rows != tmp_mat.rows || mat.cols != tmp_mat.cols ){
        DEV_INFOS("the file will be rescaled");
        return true;
    }
    return false;
}
bool Gui_MaskSetter::drawMask(){
    Gtk::Window* parent =  (Gtk::Window*) get_toplevel();
    DrawMaskDial dial(m_processor_hand,m_mask);
    dial.set_transient_for(*parent);
    if(dial.run() == Gtk::RESPONSE_OK){
        dial.getMask().copyTo(m_mask);
        return true;
    }
    else
        return false;

}
void Gui_MaskSetter::updateNFiles(int i){
    if(i == 0)
        this->hide_all();
    else
        this->show_all();
}

/*******************************************************************************************/
DrawMaskDial::DrawMaskDial(Gui_ProcessorHandler& processor_hand, MaskROI& mask):
Gtk::Dialog(),
m_processor_hand(processor_hand),
m_rb_circle(THREE_POINT_CIRCLE_TOOL),
m_rb_conv_polyg(CONVEX_POLYGON_TOOL),
m_plus(Gtk::Stock::ADD),
m_minus(Gtk::Stock::REMOVE),
display(processor_hand),
m_tool(MASK_TOOL_3P_CIRCLE)
{
    m_text.set_line_wrap();
    mask.copyTo(m_mask);
    display.setMaskPtr(&m_mask);
    this->get_vbox()->pack_start(m_hbox);
    m_text.set_markup(DRAW_MASK_TEXT);
    m_hbox.pack_start(m_vbox2,false,false);
    m_vbox2.pack_start(m_text,true,false);
    m_vbox2.pack_start(m_rb_circle,true,false);
    m_vbox2.pack_start(m_rb_conv_polyg,true,false);
    m_vbox2.pack_start(m_hbox2,true,false);
    m_hbox2.pack_start(m_plus,true,true);
    m_hbox2.pack_start(m_minus,true,true);
    Gtk::RadioButton::Group group = m_rb_circle.get_group();
    m_rb_conv_polyg.set_group(group);
    m_frame.add(display);
    m_hbox.pack_start(m_frame,true,true);
    m_vbox2.set_border_width(5);
    m_frame.set_border_width(5);
    this->get_vbox()->show_all();
    this->add_button(Gtk::Stock::OK,Gtk::RESPONSE_OK);
    this->add_button(Gtk::Stock::CANCEL,Gtk::RESPONSE_CANCEL);
    this->get_vbox()->show();
    display.signal_click_on_obj().connect(sigc::mem_fun(*this,&DrawMaskDial::on_points_changed));
    m_rb_circle.signal_toggled().connect(sigc::mem_fun(*this,&DrawMaskDial::on_change_tool));
    m_rb_conv_polyg.signal_toggled().connect(sigc::mem_fun(*this,&DrawMaskDial::on_change_tool));
    m_plus.signal_clicked().connect(sigc::mem_fun(*this,&DrawMaskDial::on_new_shape));
    m_minus.signal_clicked().connect(sigc::mem_fun(*this,&DrawMaskDial::on_remove_shape));
    m_points.push_back(std::pair<std::vector<cv::Point2f>,int >(std::vector<cv::Point2f>(),m_tool));
    updateButtons();
}
void DrawMaskDial::lockROI(){
    if(m_pts.size() >2){
        cv::Mat mat;
        if(m_processor_hand.getSourceImgAsRGB(mat)){

            if(m_points.empty())
                m_points.resize(1);

            m_points[m_points.size()-1] = std::pair<std::vector<cv::Point2f>,int >(m_pts,m_tool);
            }
        m_mask.setFromPoints(m_points,mat.cols,mat.rows);
        }
    }
void DrawMaskDial::on_points_changed(){
    m_pts = display.getPoints();
    if (m_tool == MASK_TOOL_3P_CIRCLE && m_pts.size() == 3){
        on_new_shape();
    }
    updateButtons();
}
void DrawMaskDial::on_change_tool(){
    if(m_rb_circle.get_active())
        m_tool = MASK_TOOL_3P_CIRCLE;
    else if (m_rb_conv_polyg.get_active())
        m_tool = MASK_TOOL_CONV_POLYGON;
    display.emptyPoints();
    m_pts = display.getPoints();
    updateButtons();
}
void DrawMaskDial::on_new_shape(){
    lockROI();
    m_points.push_back(std::pair<std::vector<cv::Point2f>,int >(std::vector<cv::Point2f>(),m_tool));
    display.emptyPoints();
    m_pts = display.getPoints();
    updateButtons();
}
void DrawMaskDial::updateButtons(){

    const std::vector<cv::Point2f>& pts = display.getPoints();
    if(m_tool == MASK_TOOL_3P_CIRCLE){
        if(pts.size() == 3 && !m_points.empty())
            m_plus.set_sensitive(true);
        else
            m_plus.set_sensitive(false);
    }
    else if(m_tool == MASK_TOOL_CONV_POLYGON){
        if(pts.size() >2){
            m_plus.set_sensitive();
        }
        else{
            m_plus.set_sensitive(false);
        }

    }
    if(m_points.size() < 2)
        m_minus.set_sensitive(false);
    else
         m_minus.set_sensitive(true);
}
void DrawMaskDial::on_remove_shape(){
    if(m_points.size() < 2)
        return;
    cv::Mat mat;
    if(m_processor_hand.getSourceImgAsRGB(mat)){
        display.emptyPoints();
        m_points.pop_back();
        m_points.pop_back();
        m_mask.setFromPoints(m_points,mat.cols,mat.rows);
        m_points.push_back(std::pair<std::vector<cv::Point2f>,int >(std::vector<cv::Point2f>(),m_tool));
        display.forceRedraw();
    }
    updateButtons();
}

