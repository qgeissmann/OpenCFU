#include "Step_2.hpp"

void Step_2::updateParams(const void* src,bool was_forced){
    m_block_size = 2*((int)(((cv::Mat*)src)->cols/30))+1;;


//    if(m_opts.getMask().type != MASK_TYPE_NONE)
//        cv::threshold(mat,m_old_mask,0,255,CV_THRESH_BINARY);
    m_old_thr_mode = m_opts.getThrMode();

}
bool Step_2::needReprocess(const void* src){
//    if(!this->isSameImage(m_old_mask,m_opts.getMask()))
//        return true;
    if(m_old_thr_mode != m_opts.getThrMode()){
            DEV_INFOS("New mode: "<<m_opts.getThrMode());
        return true;
    }
    return false;
}

void Step_2::makeConvolvedMask(){
    m_conv_mask=cv::Mat(m_opts.getImage().rows,m_opts.getImage().cols,CV_8U,cv::Scalar(255));
    m_old_mask=m_conv_mask;
}
void Step_2::process(const void* src){
    DEV_INFOS("Correcting brightness ...");
    this->makeConvolvedMask();
    cv::Mat invertMask;
    cv::bitwise_not(m_old_mask,invertMask);
    std::vector<cv::Mat> compos;
    cv::split(*((cv::Mat*)src),compos);
    #pragma omp parallel for schedule(static, 1)
    for(unsigned int i=0;i<compos.size();i++){
        /* Remove  masked regions*/
        compos[i] = compos[i]-invertMask;
        cv::Mat conv;
        float r = 196.0/ (float) compos[i].cols ;
        cv::resize(compos[i],conv,cv::Size(0,0),r,r,cv::INTER_AREA);
        cv::medianBlur(conv,conv,11);
        cv::resize(conv,conv,compos[i].size(),0,0,cv::INTER_LINEAR);
        if(m_old_thr_mode == OCFU_THR_NORM){
            compos[i]=255*(conv/m_conv_mask)-compos[i];
        }
        else if(m_old_thr_mode == OCFU_THR_INV){
            compos[i] = compos[i] - 255*(conv/m_conv_mask);
        }
        else if(m_old_thr_mode == OCFU_THR_BILAT){ //bilateral threshold
            cv::absdiff(compos[i],255*(conv/m_conv_mask),compos[i]);
        }
        cv::normalize(compos[i], compos[i],0,255, cv::NORM_MINMAX,-1, m_old_mask);
        this->SubstractLapOGauss(compos[i], compos[i],LAPOFGAUSS_BLUR_SIZE);
    }
    m_step_img = (compos[0]+compos[1]+compos[2])/3;
    m_step_result = (void*) &m_step_img;
    DEV_INFOS("Correcting brightness finished...");
}


//TODO polish this function !! => get rid of magic numbers ^^
void Step_2::SubstractLapOGauss(const cv::Mat& in, cv::Mat& out, int blurSize){
    cv::Mat tmp_mat;
    cv::GaussianBlur(in,tmp_mat,cv::Size(blurSize,blurSize),3);
    cv::Laplacian(tmp_mat,tmp_mat,CV_8U, 5, 0.3);
    /*find holes in laplacian and remove them*/
    std::vector<std::vector<cv::Point> > contours;
    std::vector<std::vector<cv::Point> > contoursToDraw(1);
    std::vector<cv::Vec4i > hierarchy;
    cv::Mat tmp;
    cv::threshold(tmp_mat,tmp, 10, 255, cv::THRESH_BINARY);
    cv::findContours(tmp, contours,hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);//TC89_L1);
    for(unsigned int k=0; k<contours.size();k++){
        /* if the contour has no holes and if it is not a hole*/
        if( hierarchy[k][2] < 0 && hierarchy[k][3] < 0){
            std::copy(contours.begin()+k, contours.begin()+k+1, contoursToDraw.begin());
            cv::Rect rect = cv::boundingRect(contoursToDraw[0]);
            cv::drawContours(tmp_mat,contoursToDraw,0,cv::Scalar(0),-1,8, cv::noArray(), 1,cv::Point(-rect.x,-rect.y));
        }
    }
    out = in-tmp_mat;
}
