#include "Gui_DisplayBaseClass.hpp"



Gui_DisplayBaseClass::Gui_DisplayBaseClass(Gui_ProcessorHandler& processor_hand):
    m_processor_hand(processor_hand),
    m_deco(this,processor_hand,m_show_idx,m_show_point,m_ROI),
    m_ROI(cv::Point(0,0),cv::Point(0,0)),
    m_draw_result(false),
    m_alph_mask(128),
    m_show_idx(-1),m_show_point(false),m_show_mask(true),
    m_cursor(Gdk::CROSS)
    {

    this->add_events( Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK );
    this->signal_scroll_event().connect( sigc::mem_fun( *this, &Gui_DisplayBaseClass::on_scroll) );
    this->signal_button_press_event().connect( sigc::mem_fun( *this, &Gui_DisplayBaseClass::on_click) );

    m_color.set_rgb_p(0.2,0.2,0.2);
    this->modify_bg(Gtk::STATE_NORMAL,m_color);

    m_processor_hand.signal_state().connect( sigc::mem_fun(*this,&Gui_DisplayBaseClass::myRedraw));

    Glib::RefPtr< Gdk::Screen > screen = Gdk::Screen::get_default();
    Gdk::Rectangle rect;
    screen->get_monitor_geometry(screen->get_primary_monitor(),rect);
    set_size_request(rect.get_width()/4,rect.get_height()/4);



    makeLUT();




}
void Gui_DisplayBaseClass::on_realize(){
    Gtk::DrawingArea::on_realize();
m_ref_window = this->get_window();
    assert(m_ref_window);
    m_ref_window->set_cursor(m_cursor);
}


bool Gui_DisplayBaseClass::on_expose_event(GdkEventExpose* event){
    m_ROI.reallocateSize(this->get_allocation());
    redraw();
    return true;
}

bool Gui_DisplayBaseClass::on_scroll(GdkEventScroll* event){
    cv::Point2f xy_pb = cv::Point2f(event->x,event->y);
    bool ret = m_ROI.newROIFromScroll(xy_pb, event->direction);
    this->queue_draw();
    DEV_INFOS("scrolling");
    return ret;
}

void Gui_DisplayBaseClass::myRedraw(int state){
    if(state == PROCESSOR_IDLE_SIGNAL)
        m_draw_result = true;
    else
        m_draw_result = false;
    queue_resize();
    }

void Gui_DisplayBaseClass::drawBackground(){
//    if(!m_background)
//    m_background = Gui_PixbufOpener::pixbufOpen(BACKGROUND_NOISE);
//    cv::Point2f wh = m_ROI.getPixbufDim();
//    m_background = m_background->scale_simple (wh.x, wh.x, Gdk::INTERP_TILES) ;
//    m_background->render_to_drawable(get_window(), get_style()->get_black_gc(),0, 0,
//    0, 0, wh.x, wh.x,Gdk::RGB_DITHER_NONE, 0, 0);


}
void Gui_DisplayBaseClass::redraw(){
    DEV_INFOS("redrawing");
    cv::Mat tmp_img;
    cv::Point2f offset;
    drawBackground();
    if(!m_processor_hand.getSourceImgAsRGB(tmp_img)){
        if(!m_banner_pixbuf)
            m_banner_pixbuf = Gui_PixbufOpener::pixbufOpen(BANNER_IMG);
            offset = m_ROI.scaleToFitAllocation(m_banner_pixbuf, m_pixbuf);
    }
    else{
        m_banner_pixbuf.clear();
        cv::Point2f raw_img_dim(tmp_img.cols,tmp_img.rows);
        m_ROI.update(raw_img_dim);
        tmp_img(m_ROI).copyTo(tmp_img);
        offset = m_ROI.scaleToFitAllocation(tmp_img, m_img_to_display);
        m_pixbuf = Gdk::Pixbuf::create_from_data((guint8*)m_img_to_display.data,Gdk::COLORSPACE_RGB,false,
                                                8,m_img_to_display.cols,m_img_to_display.rows,m_img_to_display.step);




        //show mask

        if(getMask(tmp_img) && m_show_mask){

            DEV_INFOS("drawing mask "<<cv::Point2f(tmp_img.rows,tmp_img.cols)<<" vs "<<raw_img_dim);
            tmp_img(m_ROI).copyTo(tmp_img);
            cv::LUT(tmp_img,m_LUT,tmp_img);

            m_ROI.scaleToFitAllocation(tmp_img, tmp_img,true);

            m_mask_pixbuf = Gdk::Pixbuf::create_from_data((guint8*)tmp_img.data,Gdk::COLORSPACE_RGB,false,
                                                    8,tmp_img.cols,tmp_img.rows,tmp_img.step);
            m_mask_pixbuf = m_mask_pixbuf->add_alpha(true,0,0,0);
            m_mask_pixbuf->composite 	( 	m_pixbuf,0,0,
                m_mask_pixbuf->get_width(),m_mask_pixbuf->get_height(),
                0,0,1,1,Gdk::INTERP_NEAREST,
                m_alph_mask//int  	overall_alpha
                );


        }


    }
    m_pixbuf->render_to_drawable(get_window(), get_style()->get_black_gc(),0, 0,
                  offset.x, offset.y, m_pixbuf->get_width(), m_pixbuf->get_height(),Gdk::RGB_DITHER_NONE, 0, 0);

    if(m_draw_result)
        m_deco.decorate();

}
void Gui_DisplayBaseClass::makeLUT(){

    m_LUT=cv::Mat(1,256,CV_8UC3,cv::Scalar(1,1,1));

    std::vector<cv::Vec3b> cols;
    cols.push_back(cv::Vec3b(255,0,0));
    cols.push_back(cv::Vec3b(0,0,255));
    cols.push_back(cv::Vec3b(0,255,0));
    cols.push_back(cv::Vec3b(255,255,0));
    cols.push_back(cv::Vec3b(255,0,255));
    cols.push_back(cv::Vec3b(0 ,255,255));
    cols.push_back(cv::Vec3b(128 ,128,255));
    cols.push_back(cv::Vec3b(255,128 ,128));
    cols.push_back(cv::Vec3b(128 ,255,128));
    unsigned int N = cols.size();

    for(unsigned int i = 1; i<256;++i)
        for(unsigned int j =0; j<3;++j)
            m_LUT.at<cv::Vec3b>(i)[j] = cols[(i-1)%N][j];
}
