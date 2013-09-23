#ifndef GUI_DISPLAYROI_H
#define GUI_DISPLAYROI_H
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <gtkmm/widget.h>
#include "defines.hpp"

class Gui_DisplayRoi: public cv::Rect
{
    public:
        Gui_DisplayRoi(cv::Point pt1, cv::Point pt2) : cv::Rect(pt1,pt2),m_zoom(1), m_zoom_incr(1.2){}
        void reallocateSize(Gtk::Allocation alloc);
        cv::Point2f scaleToFitAllocation(const cv::Mat& src, cv::Mat& dst,bool interp_nearest = false);
        cv::Point2f scaleToFitAllocation(const Glib::RefPtr<Gdk:: Pixbuf >& src, Glib::RefPtr<Gdk:: Pixbuf>& dst);
        bool newROIFromScroll(cv::Point2f xy_pb,int direc);
        void makeNewROI(const cv::Point2f center,bool put_in_center);
        void update(cv::Point2f raw_dim){
            m_raw_dim = raw_dim;
            if(this->br().x > m_raw_dim.x || this->br().y > m_raw_dim.y)
                m_zoom = 1;
            if(m_zoom == 1){
                x = 0;
                y = 0;
                width = m_raw_dim.x;
                height = m_raw_dim.y;
                }
        }
        void getABForDeco(cv::Point2f& a,cv::Point2f& b){
            b = CoordPbToROI(CoordROIToRaw(cv::Point2f(0,0),true),m_pixbuf_wh,true);
            a = CoordPbToROI(CoordROIToRaw(m_raw_dim,true),m_pixbuf_wh,true) - b;
            a.x /= m_raw_dim.x;
            a.y /= m_raw_dim.y;
        }
        cv::Point2f pbToRaw(cv::Point2f p){
            return CoordROIToRaw(CoordPbToROI(p,m_pixbuf_wh,false),false);
        }
        cv::Point2f getPixbufDim(){
            return m_pixbuf_wh;
        }
    protected:

        float calcScale(const cv::Point2f img_wh, const cv::Point2f pb_wh);
        cv::Point2f calcOffset(float scale_ratio, const cv::Point2f img_wh, const cv::Point2f pb_wh);

        cv::Point2f CoordPbToROI(const cv::Point2f xy,const cv::Point2f pb_wh,const  bool revers);
        cv::Point2f CoordROIToRaw(const cv::Point2f xy,const  bool revers);


    private:
        cv::Point2f m_pixbuf_wh;
        float m_zoom;
        float m_zoom_incr;
        cv::Point2f m_raw_dim;


};

#endif // GUI_DISPLAYROI_H
