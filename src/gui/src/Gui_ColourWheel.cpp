#include "Gui_ColourWheel.hpp"

Gui_ColourWheel::Gui_ColourWheel(){
    Glib::RefPtr< Gdk::Screen > screen = Gdk::Screen::get_default();
    Gdk::Rectangle rect;
    screen->get_monitor_geometry(screen->get_primary_monitor(),rect);
    set_size_request(rect.get_width()/32,rect.get_height()/32);
    m_pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB,true,8,1,1);
}


bool Gui_ColourWheel::on_expose_event(GdkEventExpose* event){
    this->redraw();
    return true;
}

void Gui_ColourWheel::setParams(int mean_hue, int tol_hue, int min_sat, int max_sat){
    m_centr_hue = mean_hue;
    m_tol_hue = tol_hue;
    m_min_sat = min_sat;
    m_max_sat = max_sat;
    queue_draw();
}

void Gui_ColourWheel::redraw(){
    Gtk::Allocation allocation = this->get_allocation();
    const float pixbuf_w = allocation.get_width();
    const float pixbuf_h = allocation.get_height();
    int siz = std::min(pixbuf_w,pixbuf_h);
    m_img_to_display = cv::Mat(siz*2+1,siz*2+1,CV_8UC3,cv::Scalar(128,128,128));
    cv::cvtColor(m_img_to_display,m_img_to_display,cv::COLOR_BGR2HLS);

    std::vector<cv::Mat> chanels;
    cv::split(m_img_to_display, chanels);

    int h = m_img_to_display.rows;
    int w = m_img_to_display.cols;

    for( int i = 0; i < h; i++ ){
        for( int j = 0; j < w; j++ ){

            int mean_hue = std::atan2((h/2)-i , (w/2)-j) * 180/MY_PI;
            if (mean_hue < 0 )
                mean_hue = (180 + mean_hue) +180 ;

            int val =((h/2) - i) *((h/2) - i) + ((w/2) - j) *((w/2) - j);
            val = std::sqrt(val);
            val = val*255/(h/2);

            chanels[0].at<uchar>(i, j) = mean_hue/2;
            chanels[2].at<uchar>(i, j) = (uchar) val;

            float aa = (float) mean_hue *  MY_PI /180;
            float bb = (float) m_centr_hue *  MY_PI /180;

            int diff = (int) fabs(atan2(sin(aa-bb), cos(aa-bb)) * 180 / MY_PI);
            if(val < m_min_sat || val > m_max_sat || diff > m_tol_hue){
                chanels[1].at<uchar>(i, j) = 0;
                chanels[2].at<uchar>(i, j) = 0;
            }

        }
    }

    cv::merge(chanels,m_img_to_display);
    cv::cvtColor(m_img_to_display,m_img_to_display,cv::COLOR_HLS2BGR);
//    cv::GaussianBlur(m_img_to_display,m_img_to_display,cv::Size(3,3),1.5);
    cv::resize(m_img_to_display,m_img_to_display,cv::Size(siz,siz),0,0,cv::INTER_NEAREST);
    cv::cvtColor(m_img_to_display,m_img_to_display,cv::COLOR_BGR2RGB);

    h = m_img_to_display.rows;
    w = m_img_to_display.cols;
    if(pixbuf_w != m_pixbuf->get_width() || pixbuf_h != m_pixbuf->get_height() )
        m_pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB,true,8,pixbuf_w,pixbuf_h);
    m_pixbuf->fill(0x00000000);

    Glib::RefPtr<Gdk:: Pixbuf > tmp_buf= Gdk::Pixbuf::create_from_data(
                                            (guint8*)m_img_to_display.data,Gdk::COLORSPACE_RGB,false,
                                            8,w,h,m_img_to_display.step);

    tmp_buf = tmp_buf->add_alpha(true,0,0,0);
    tmp_buf->composite 	( 	m_pixbuf,0,0,
        w,h,0,0,1,1,Gdk::INTERP_NEAREST,255);

    m_pixbuf->render_to_drawable(get_window(), get_style()->get_black_gc(),0, 0,
                                  0, 0, m_pixbuf->get_width(),m_pixbuf->get_height(),Gdk::RGB_DITHER_NONE, 0, 0);

}
