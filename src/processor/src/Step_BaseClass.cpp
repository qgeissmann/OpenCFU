#include "Step_BaseClass.hpp"

Step_BaseClass::Step_BaseClass(ProcessingOptions& opts):
m_opts(opts){}

Step_BaseClass::~Step_BaseClass(){}

bool Step_BaseClass::run(void*& inout,bool forceReprocess){
    DEV_INFOS("" + std::string(typeid(*this).name()));
    bool need_reprocess = this->needReprocess(inout);
    bool toReprocess = need_reprocess || forceReprocess ;
    if (toReprocess ){
        this->updateParams(inout,forceReprocess);
        DEV_INFOS("Processing step started from class:" + std::string(typeid(*this).name()));
        this->process(inout);
    }
    inout = m_step_result;
    return toReprocess;
}

bool Step_BaseClass::isSameImage(const cv::Mat& img1,const cv::Mat& img2){
    if (img1.empty() != img2.empty()){
        DEV_INFOS("Diff Img: one is empty");
        return false;
    }
    else if (img1.empty()){
        DEV_INFOS("BOTH EMPTY");
        return true;
    }
    else if (img1.channels() != img2.channels()){
        DEV_INFOS("Diff Img: not the same nb of channels!");
        return false;
    }
    else if (img1.rows != img2.rows){
        DEV_INFOS("Diff Img: not the same height!");
        return false;
    }
    else if (img1.cols != img2.cols){
        DEV_INFOS("Diff Img: not the same height!");
        return false;
    }

    cv::Mat tmp_mat;
    cv::compare(img1, img2,tmp_mat,cv::CMP_NE);
    if (tmp_mat.channels() == 3)
        cv::cvtColor(tmp_mat,tmp_mat,CV_BGR2GRAY);
    int n = cv::countNonZero(tmp_mat);
    if(n > 0){
        DEV_INFOS("Diff Img: different pixels");
        return false;
    }
    DEV_INFOS("Similar images !");
    return true;

}
