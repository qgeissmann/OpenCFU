#ifndef GUI_COLOURWHEEL_H
#define GUI_COLOURWHEEL_H

#include <gtkmm/drawingarea.h>
#include "defines.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class Gui_ColourWheel: public Gtk::DrawingArea
{
    public:
        Gui_ColourWheel();
        void setParams(int mean_hue, int tol_hue, int min_sat, int max_sat);
    protected:
        bool on_expose_event(GdkEventExpose* event);
        void redraw();
    private:
        cv::Mat  m_img_to_display;
        Glib::RefPtr<Gdk:: Pixbuf > m_pixbuf;
//        Glib::RefPtr<Gdk:: Pixbuf > tmp_buf;
        int m_centr_hue;
        int m_tol_hue;
        int m_min_sat;
        int m_max_sat;

};

#endif // GUI_COLOURWHEEL_H
