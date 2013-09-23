#include "Gui_DisplayRoi.hpp"



void Gui_DisplayRoi::reallocateSize(Gtk::Allocation alloc){
            m_pixbuf_wh.x = alloc.get_width();
            m_pixbuf_wh.y = alloc.get_height();
        };



cv::Point2f Gui_DisplayRoi::CoordPbToROI(const cv::Point2f xy, const cv::Point2f pb_wh ,const  bool revers){
    float r = calcScale(cv::Point2f(width,height),pb_wh );
    cv::Point2f off = calcOffset(r,cv::Point2f(width,height),pb_wh );
    if(!revers)
        return (xy - off ) * r;
    else
        return off + xy * (1/r);
}
cv::Point2f Gui_DisplayRoi::CoordROIToRaw(const cv::Point2f xy,const  bool revers){
    cv::Point2f pt = tl();
    if(!revers)
        return xy + pt;
    else
        return xy - pt;
}

cv::Point2f Gui_DisplayRoi::scaleToFitAllocation(const Glib::RefPtr<Gdk:: Pixbuf >& src, Glib::RefPtr<Gdk:: Pixbuf>& dst){
    int w = src->get_width ();
    int h = src->get_height ();
    float r = calcScale(cv::Point2f(w,h),m_pixbuf_wh);
    cv::Point2f off = calcOffset(r,cv::Point2f(w,h),m_pixbuf_wh);
    dst = src->scale_simple(w/r,h/r,Gdk::INTERP_BILINEAR);
    return off;
}
cv::Point2f Gui_DisplayRoi::scaleToFitAllocation(const cv::Mat& src, cv::Mat& dst,bool interp_nearest){

    float r = calcScale(cv::Point2f(src.cols,src.rows),m_pixbuf_wh);
    if(!interp_nearest)
        cv::resize(src,dst,cv::Size(),1/r,1/r,cv::INTER_AREA);
    else
        cv::resize(src,dst,cv::Size(),1/r,1/r,cv::INTER_NEAREST);

    return calcOffset(r,cv::Point2f(src.cols,src.rows),m_pixbuf_wh);
}
//
float Gui_DisplayRoi::calcScale(const cv::Point2f img_wh, const cv::Point2f pb_wh){
    float r_img = img_wh.x/img_wh.y;
    float r_pb = pb_wh.x/pb_wh.y;

    if(r_pb>r_img)
       return img_wh.y/(pb_wh.y - 2*DISPLAY_MIN_MARGIN);

    else
        return img_wh.x/(pb_wh.x- 2*DISPLAY_MIN_MARGIN);
}
cv::Point2f Gui_DisplayRoi::calcOffset(float scale_ratio, const cv::Point2f img_wh, const cv::Point2f pb_wh){
    float r_img = img_wh.x/img_wh.y;
    float r_pb = pb_wh.x/pb_wh.y;

    if(r_pb>r_img){
        int x = (pb_wh.x - img_wh.x/scale_ratio) /2;
        return cv::Point2f(x,DISPLAY_MIN_MARGIN);

    }
    else{
        int y = (pb_wh.y - img_wh.y/scale_ratio )/2;
        return cv::Point2f(DISPLAY_MIN_MARGIN,y);
    }
}

//
void Gui_DisplayRoi::makeNewROI(const cv::Point2f center,bool put_in_center){
    cv::Point2f wh(m_raw_dim.x/m_zoom, m_raw_dim.y/m_zoom);
    cv::Point2f pos_in_roi = CoordROIToRaw(center,true);

    float rx;
    float ry;

    if(!put_in_center){
        cv::Point2f excent = (br() - tl() );
        rx = pos_in_roi.x/excent.x;
        ry = pos_in_roi.y/excent.y;
    }
    else{
        rx = 0.5;
        ry = 0.5;
    }

    float xb = center.x + wh.x * (1 - rx);
    float yb = center.y + wh.y * (1 - ry);
    float xa = center.x - wh.x * (rx);
    float ya = center.y - wh.y * (ry);
    cv::Point2f top_left = cv::Point2f(xa,ya) ;
    cv::Point2f  bot_right = cv::Point2f(xb,yb);

    if(top_left.x < 0){
        bot_right = bot_right - cv::Point2f(top_left.x,0);
        top_left = top_left - cv::Point2f(top_left.x,0);
    }

    if(top_left.y < 0){
        bot_right = bot_right - cv::Point2f(0,top_left.y);
        top_left = top_left - cv::Point2f(0,top_left.y);
    }

    if(bot_right.x > m_raw_dim.x){
        float dx = bot_right.x - m_raw_dim.x;
        top_left = top_left - cv::Point2f(dx,0);
        bot_right = bot_right - cv::Point2f(dx,0);
    }

    if(bot_right.y > m_raw_dim.y){
        float dy = bot_right.y - m_raw_dim.y;
        top_left = top_left - cv::Point2f(0,dy);
        bot_right = bot_right - cv::Point2f(0,dy);
    }
    this->x = top_left.x;

    this->y = top_left.y;
    this->width = bot_right.x - top_left.x;
    this->height = bot_right.y - top_left.y;

}

bool Gui_DisplayRoi::newROIFromScroll(cv::Point2f xy_pb,int direc ){

    cv::Point2f xy_raw = CoordROIToRaw( CoordPbToROI(xy_pb,m_pixbuf_wh,false),false);
    if(xy_raw.x > m_raw_dim.x)
        xy_raw.x = m_raw_dim.x;
    else if (xy_raw.x < 0)
        xy_raw.x = 0;
    if(xy_raw.y > m_raw_dim.y)
        xy_raw.y = m_raw_dim.y;
    else if (xy_raw.y < 0)
        xy_raw.y = 0;
    if(direc == GDK_SCROLL_UP){
        if(width > 24 && height > 24)
        m_zoom *= m_zoom_incr;
    }
    else if(direc == GDK_SCROLL_DOWN){
        m_zoom /= m_zoom_incr;
        if(m_zoom < 1)
            m_zoom = 1;
    }
    makeNewROI(xy_raw,false);
    return true;
}

/**
bool Gui_Display::on_scroll(GdkEventScroll* event){
    cv::Point2f xy_pb = cv::Point2f(event->x,event->y);
    cv::Point2f xy_raw = CoordROIToRaw(CoordPbToROI(xy_pb));
    if(xy_raw.x > m_raw_img_dim.x)
        xy_raw.x = m_raw_img_dim.x;
    else if (xy_raw.x < 0)
        xy_raw.x = 0;
    if(xy_raw.y > m_raw_img_dim.y)
        xy_raw.y = m_raw_img_dim.y;
    else if (xy_raw.y < 0)
        xy_raw.y = 0;
    if(event->direction == GDK_SCROLL_UP){
        if(m_ROI.width > 24 && m_ROI.height > 24)
        m_zoom *= m_zoom_incr;
    }
    else if(event->direction == GDK_SCROLL_DOWN){
        m_zoom /= m_zoom_incr;
        if(m_zoom < 1)
            m_zoom = 1;
    }
    makeNewROI(xy_raw);
    this->queue_draw();
    return true;
}
*/
