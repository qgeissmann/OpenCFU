#ifndef GUI_MASKSETTER_H
#define GUI_MASKSETTER_H

#include "defines.hpp"
#include "Gui_OptionSetterBaseClass.hpp"
#include "Gui_DisplayBaseClass.hpp"
#include "text.hpp"
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/stock.h>
#include <glibmm/miscutils.h>
#include <giomm/file.h>
#include "MaskROI.hpp"
#include <gtkmm/comboboxtext.h>




class MyComboboxText : public Gtk::ComboBoxText{
    public:
        MyComboboxText():Gtk::ComboBoxText(),m_last_idx(0),m_revert_switch(false){

        }

        int getLastIxd(){return m_last_idx;}

        void revert(){
            if(get_active() != m_last_idx)
               set_active(m_last_idx);
        }
        void append(int ID, const std::string& str){
            m_data_map[str] = ID;
            append_text(str);
        }
    int getState(){return m_data_map[get_active_text()];}
    sigc::signal<bool,int> signal_select_type(){return m_signal_select_type;}
    sigc::signal<void,int> signal_change_state(){return m_signal_change_state;}
    protected:
        void on_changed(){
                int state = getState();
                if(!m_revert_switch){
                    if(m_signal_select_type.emit(state)){
                        m_last_idx = get_active_row_number();
                        DEV_INFOS(m_last_idx);
                        m_signal_change_state.emit(state);
                        Gtk::ComboBoxText::on_changed();
                    }
                    else{
                        m_revert_switch = true;
                        set_active(m_last_idx);

                    }
                }
                else{
                    m_revert_switch = false;
                }
        }

    void on_realize(){
        m_revert_switch = true;
        set_active(0);
        Gtk::ComboBoxText::on_realize();
    }

    private:
        sigc::signal<bool,int> m_signal_select_type;
        sigc::signal<void,int> m_signal_change_state;
        std::map<std::string,int> m_data_map;
        int m_last_idx;
        bool m_revert_switch;
};



class Gui_MaskSetter : public Gui_OptionSetterBaseClass{
    public:
        Gui_MaskSetter(Gui_ProcessorHandler& processor_hand,const std::string str);
        void updateNFiles(int i);
        sigc::signal<void,bool> signal_show_mask(){return m_signal_show_mask;}
    protected:
        bool updateOptions();
        bool on_AddFileClicked();
        bool drawMask();
        bool checkFile(const Glib::RefPtr<Gio::File>& file);
        bool on_combo_changed(int state);
        void on_update_hbox(int state);
        void on_remake_mask();
        void on_show_mask(){m_signal_show_mask.emit(m_check_butt.get_active());}
    private:
        Gtk::HBox m_hbox;
        Glib::RefPtr<Gio::File> m_file;
        std::string m_last_dir;
        Gtk::FileFilter m_img_filter, m_no_filter;
        MyComboboxText m_combo;
        Gtk::Button m_remake_button;
        Gtk::Label m_from_file_label;
        MaskROI m_mask;
        Gtk::CheckButton m_check_butt;
        sigc::signal<void,bool> m_signal_show_mask;
};


class Mask_Display : public Gui_DisplayBaseClass
{
public:
    Mask_Display(Gui_ProcessorHandler& processor_hand):
        Gui_DisplayBaseClass(processor_hand)
        {
        m_cursor = Gdk::Cursor(Gdk::PENCIL);
        myRedraw(PROCESSOR_IDLE_SIGNAL);
        m_deco.setVectorOfCrosses(&m_points);

    Glib::RefPtr< Gdk::Screen > screen = Gdk::Screen::get_default();
    Gdk::Rectangle rect;
    screen->get_monitor_geometry(screen->get_primary_monitor(),rect);
    set_size_request(rect.get_width()*0.7,rect.get_height()*0.7);

    };
    void setMaskPtr(MaskROI* mask){m_mask = mask;}
    void forceRedraw(){queue_draw();}
    void emptyPoints(){m_points.resize(0);queue_draw();}
    const std::vector<cv::Point2f>& getPoints(){return m_points;}
    sigc::signal<void>& signal_click_on_obj(){return m_signal_click_on_obj;}

    protected:
        bool on_click(GdkEventButton* event){
            if(event->button <3){
               cv::Point2f p = m_ROI.pbToRaw(cv::Point2f(event->x,event->y));
               m_points.push_back(p);
               DEV_INFOS((int)m_draw_result);
            }
            else{
                if (!m_points.empty())
                    m_points.pop_back();
            }
            queue_draw();
            m_signal_click_on_obj.emit();
            return true;
            }
            bool getMask(cv::Mat& tmp_img){
                DEV_INFOS("GEtTING MASK");

                m_processor_hand.getSourceImgAsRGB(tmp_img);
                m_mask->update(tmp_img);
                const cv::Mat&  mask_img = m_mask->getMat();
                DEV_INFOS("GEtTING MASK2");
                if(mask_img.empty()){
                    DEV_INFOS("empty mask");
                    return false;
                }
                else{
                    DEV_INFOS("ok mask");
                    cv::cvtColor(mask_img,tmp_img,cv::COLOR_GRAY2RGB);
                    DEV_INFOS("RGB mask");
                    return true;
                }
            }

    private:
        std::vector<cv::Point2f> m_points;
        sigc::signal<void> m_signal_click_on_obj;
        MaskROI* m_mask;
};


class DrawMaskDial: public Gtk::Dialog
{
    public:
        DrawMaskDial(Gui_ProcessorHandler& processor_hand, MaskROI& mask);
        const MaskROI& getMask()const {return  m_mask;}
    protected:
        void on_change_tool();
        void on_points_changed();
        void lockROI();
        void on_new_shape();
        void on_remove_shape();
        void updateButtons();
    private:
        Gui_ProcessorHandler& m_processor_hand;
        Gtk::RadioButton m_rb_circle, m_rb_conv_polyg;
        Gtk::Button m_plus, m_minus;
        Gtk::RadioButton::Group group;
        Mask_Display display;
        int m_tool;

        Gtk::HBox m_hbox;
        Gtk::HBox m_hbox2;
        Gtk::VBox m_vbox2;
        Gtk::Label m_text;
        std::vector< std::pair<std::vector<cv::Point2f>,int > > m_points;
        std::vector<cv::Point2f> m_pts;
        MaskROI m_mask;
        Gtk::Frame m_frame;


};


#endif // GUI_MASKSETTER_H
