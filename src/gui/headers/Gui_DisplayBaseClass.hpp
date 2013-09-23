#ifndef GUI_DISPLAYBASECLASS_H
#define GUI_DISPLAYBASECLASS_H


#include <gtkmm/drawingarea.h>
#include <gtkmm/frame.h>
#include <gdkmm/cursor.h>
#include "opencv2/core/core.hpp"
#include "defines.hpp"
#include "Result.hpp"
#include "Gui_ProcessorHandler.hpp"
#include "Gui_Decorator.hpp"
#include "Gui_DisplayRoi.hpp"
#include "Gui_PixbufOpener.hpp"

class Gui_DisplayBaseClass : public Gtk::DrawingArea
{
    public:
        Gui_DisplayBaseClass(Gui_ProcessorHandler& processor_hand);

    protected:
        void on_realize();
        void drawBackground();
        void makeLUT();
        virtual bool on_click(GdkEventButton* event)=0;
        bool on_scroll(GdkEventScroll* event);
        bool on_expose_event(GdkEventExpose* event);
        void myRedraw(int state);
        void redraw();
        virtual bool getMask(cv::Mat& tmp_img){

            if(m_processor_hand.getMaskAsRGB(tmp_img))
                return true;
            else
                return false;

        }


        Gui_ProcessorHandler& m_processor_hand;
        Gui_Decorator m_deco;
        cv::Mat m_img_to_display;

        cv::Mat m_LUT;
        Gui_DisplayRoi m_ROI;
        Glib::RefPtr<Gdk:: Pixbuf > m_pixbuf;
        Glib::RefPtr<Gdk:: Pixbuf > m_mask_pixbuf;
        Glib::RefPtr<Gdk:: Pixbuf > m_banner_pixbuf;
        Glib::RefPtr<Gdk:: Pixbuf > m_background;
        bool m_draw_result;
        Gdk::Color m_color;

        int m_alph_mask;
        std::string m_banner_path;

        int m_show_idx;
        bool m_show_point;
        bool m_show_mask;
        Gdk::Cursor m_cursor;
        Glib::RefPtr <Gdk::Window> m_ref_window;

};

#endif // GUI_DISPLAY_H
