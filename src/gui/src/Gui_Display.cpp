#include "Gui_Display.hpp"
//#include <sstream>
#include "Gui_PixbufOpener.hpp"

Gui_Display::Gui_Display(Gui_ProcessorHandler& processor_hand):
    Gui_DisplayBaseClass(processor_hand)
    {}



void Gui_Display::setShowDeco(bool show,double line_width){
    m_deco.setLineWidthMult(line_width);
    m_draw_result = show;
    queue_resize();
}

void Gui_Display::show_this_idx(int idx){
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
bool Gui_Display::on_click(GdkEventButton* event){

    cv::Point2f p = m_ROI.pbToRaw(cv::Point2f(event->x,event->y));

    int idx = -1;

    float min_dist = INT_MAX;

    const Result& result = m_processor_hand.getNumResult();
    for(unsigned int i = 0; i<result.size();i++){
        const cv::RotatedRect& rrect= result.getRow(i).getRrect();
        cv::Rect rect = rrect.boundingRect();
        if(rect.contains(p)){
            cv::Point2f pts[4];
            rrect.points(pts);
            cv::Point2f center = (pts[0] + pts[2]) *0.5;
            float dist = sqrt((center.x - p.x)*(center.x - p.x) + (center.y - p.y)*(center.y - p.y));
            if(dist < min_dist){
                idx = i;
                min_dist = dist;
            }
        }
    }
    m_signal_click_on_obj.emit(idx,event->button);
    return true;
}

