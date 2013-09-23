#ifndef GUI_DISPLAY_H
#define GUI_DISPLAY_H

#include "Gui_DisplayBaseClass.hpp"
class Gui_Display :public Gui_DisplayBaseClass
{
public:
    Gui_Display(Gui_ProcessorHandler& processor_hand);

    void setShowDeco(bool show,double line_width);
    void show_this_idx(int idx);

    sigc::signal<void,int,int>& signal_click_on_obj(){return m_signal_click_on_obj;}
    void setShowMask(bool show){m_show_mask = show; queue_draw();}
    protected:
        bool on_click(GdkEventButton* event);


    private:
        sigc::signal<void,int,int> m_signal_click_on_obj;

};


class Gui_DisplayFrame : public Gtk::Frame{
    public:
        Gui_DisplayFrame(Gui_ProcessorHandler& processor_hand,sigc::signal<void,bool,double> signal_show_decor,sigc::signal<void,bool> signal_show_mask):
        m_display(processor_hand){
            DEV_INFOS("building graphical display frame");
            this->add(m_display);
            signal_show_decor.connect( sigc::mem_fun(this,&Gui_DisplayFrame::on_show_deco));
            signal_show_mask.connect( sigc::mem_fun(this,&Gui_DisplayFrame::on_show_mask));
        }
        void on_show_deco(bool show,double line_width){
            m_display.setShowDeco(show,line_width);
        }
        void on_show_mask(bool show){
            m_display.setShowMask(show);
        }
        void on_show_specific_object(int idx){
            m_display.show_this_idx(idx);
            DEV_INFOS("asking to draw the point");
        }
        sigc::signal<void,int,int>& signal_click_on_obj(){
            return m_display.signal_click_on_obj();
            }
    private:
        Gui_Display m_display;
};

/*
#include <gtkmm/drawingarea.h>
#include <gtkmm/frame.h>
#include "opencv2/core/core.hpp"
#include "defines.hpp"
#include "Result.hpp"
#include "Gui_ProcessorHandler.hpp"
#include "Gui_Decorator.hpp"
#include "Gui_DisplayRoi.hpp"
#include "Gui_MouseHandler.hpp"


class Gui_Display : public Gtk::DrawingArea
{
    public:
        Gui_Display(Gui_ProcessorHandler& processor_hand);
        void show_this_idx(int idx){
            if(idx <0){
               m_show_point = false;
            }
            else{
                m_show_point = true;
                m_show_idx = idx;
//
                const Result& res = m_processor_hand.getNumResult();
                res.getRow(m_show_idx);
                cv::Point2f center((res.getRow(m_show_idx).getPoint(0) + res.getRow(m_show_idx).getPoint(2)) *0.5);
                m_ROI.makeNewROI(center,true);
            }
            queue_draw();
        }

        sigc::signal<void,int,int>& signal_click_on_obj(){return m_signal_click_on_obj;}

        void setShowDeco(bool show,double line_width){
            m_deco.setLineWidthMult(line_width);
            m_draw_result = show;
            queue_resize();
        }

    protected:
        void makeLUT();
        bool on_click(GdkEventButton* event);
        bool on_scroll(GdkEventScroll* event);
        bool on_expose_event(GdkEventExpose* event);
        void myRedraw(int state){
            if(state == PROCESSOR_IDLE_SIGNAL)
                m_draw_result = true;
            else
                m_draw_result = false;
            queue_resize();
            }
        void redraw();

    private:
        Gui_ProcessorHandler& m_processor_hand;
        Gui_Decorator m_deco;
        cv::Mat m_img_to_display;
//        cv::Mat m_mask_to_display;
        cv::Mat m_LUT;
        Gui_DisplayRoi m_ROI;
        Glib::RefPtr<Gdk:: Pixbuf > m_pixbuf;
        Glib::RefPtr<Gdk:: Pixbuf > m_mask_pixbuf;
        Glib::RefPtr<Gdk:: Pixbuf > m_banner_pixbuf;
        bool m_draw_result;
        Gdk::Color m_color;
        int m_show_idx;
        int m_alph_mask;
        bool m_show_point;
        sigc::signal<void,int,int> m_signal_click_on_obj;
//        std::string m_banner_path;
//        sigc::signal<void,double> m_signal_change_line_width;
        Gui_MouseHandler m_mouse_handl;

};

class Gui_DisplayFrame : public Gtk::Frame{
    public:
        Gui_DisplayFrame(Gui_ProcessorHandler& processor_hand,sigc::signal<void,bool,double> signal_show_decor):
        m_display(processor_hand){
            DEV_INFOS("building graphical display frame");
            this->add(m_display);
            signal_show_decor.connect( sigc::mem_fun(this,&Gui_DisplayFrame::on_show_deco));
        }
        void on_show_deco(bool show,double line_width){
            m_display.setShowDeco(show,line_width);
        }
        void on_show_specific_object(int idx){
            m_display.show_this_idx(idx);
            DEV_INFOS("asking to draw the point");
        }
        sigc::signal<void,int,int>& signal_click_on_obj(){
            return m_display.signal_click_on_obj();
            }
    private:
        Gui_Display m_display;
};

*/
#endif // GUI_DISPLAY_H
