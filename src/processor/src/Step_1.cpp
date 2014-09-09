#include "Step_1.hpp"

void Step_1::updateKernelSize(){
    int s = std::min(m_old_img.cols/3,m_old_img.rows/3);
    if (s  < m_old_min_rad)
            m_old_min_rad = s;


    if (m_old_min_rad > 1)
        m_kernel_size = (((m_old_min_rad -1)/4)*2)+1;
    else
        m_kernel_size = 0;
}

void Step_1::updateParams(const void* src,bool was_forced){
    DEV_INFOS("updating parms?");
    ((cv::Mat*)src)->copyTo(m_old_img);
    m_old_min_rad = m_opts.getMinMaxRad().first;
    this->updateKernelSize();
}

bool Step_1::needReprocess(const void* src){
    DEV_INFOS("need to reprocess?");
    if(m_old_img.empty()){
        DEV_INFOS("Yes: no old img");
        return true;
    }
    else if (!this->isSameImage(m_old_img,*((cv::Mat*) src))){
        DEV_INFOS("Yes: different img");
        return true;
    }
    else if(m_old_min_rad != m_opts.getMinMaxRad().first){
        DEV_INFOS("Yes: different radius");
        return true;
    }
    return false;
}

void Step_1::process(const void* src){
    DEV_INFOS("Step 1, removing noise...");

    const cv::Mat in = (*((cv::Mat*)src));
    cv::Mat tmp_mat;
    if(m_kernel_size > 1 ){
        cv::medianBlur(in,tmp_mat,1);
        cv::GaussianBlur(tmp_mat,m_step_img,cv::Size(m_kernel_size,m_kernel_size),5);
        }
    else{
        in.copyTo(m_step_img);
    }
    m_step_result = (void*) (&m_step_img);

    DEV_INFOS("Step 1 finished...");
}

