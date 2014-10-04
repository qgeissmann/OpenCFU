#ifndef GUI_DECORATOR_H
#define GUI_DECORATOR_H


#include "defines.hpp"

#include "Gui_ProcessorHandler.hpp"
#include "Result.hpp"

#include <gtkmm/drawingarea.h>
#include <cairomm/context.h>
#include <gtkmm/drawingarea.h>
#include "Gui_DisplayRoi.hpp"


class Gui_Decorator
{
    public:
        Gui_Decorator(Gtk::DrawingArea* parent, Gui_ProcessorHandler& processor_hand, int& idx_point_to_show, bool& show_point, Gui_DisplayRoi& ROI);
//        Gui_Decorator(Gtk::DrawingArea* parent, Gui_ProcessorHandler& processor_hand,Gui_DisplayRoi& ROI);
        void decorate();
        void setLineWidthMult(double mult){m_line_width_multip = mult;}
        void setVectorOfCrosses (std::vector<cv::Point2f> *vec){
            m_vector_crosses = vec;
        };
    protected:
        void drawCrosses(Cairo::RefPtr<Cairo::Context> cr);
    private:
        Gtk::DrawingArea* m_parent;
        Gui_ProcessorHandler& m_processor_hand;
        int& m_idx_point_to_show;
        bool& m_show_point;
        Gui_DisplayRoi& m_ROI;
        double m_line_width_multip;
        std::vector<cv::Point2f> *m_vector_crosses;

        void highlightCells(Cairo::RefPtr<Cairo::Context> cr, std::vector<int> cells, float r, float g, float b, float a, float width);


};

#endif // GUI_DECORATOR_H
