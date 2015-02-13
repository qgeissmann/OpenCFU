#include "ProcessingOptions.hpp"
#include "opencv2/imgproc/imgproc.hpp"


ProcessingOptions::ProcessingOptions():
    m_image(cv::Mat()),
    m_mask(cv::Mat()),
    m_min_max_radius(3,50),
    m_cent_tol_hue(180,180),
    m_min_max_sat(0,255),
    m_likelihood_thr(30),
    m_clustering_distance(2.3), //NJL 10/AUG/2014
    m_clustering_min_pts(4), //NJL 13/FEB/2015
    m_threshold (10),
    m_threshold_mode(OCFU_THR_NORM),
    m_has_max_radius(false),
    m_has_auto_threshold(false),
    m_has_hue_filter(false),
    m_has_outlier_filter(false),
    m_has_clustering_distance(false), //NJL 10/AUG/2014
    m_gui_filter(new cv::Mat){
}

//ProcessingOptions::~ProcessingOptions(){
//    DEV_INFOS("destroying");
//}

ProcessingOptions&  ProcessingOptions::operator= (const ProcessingOptions& cpy){
    m_min_max_radius = cpy.getMinMaxRad();
    m_cent_tol_hue = cpy.getCenTolHue();

    m_min_max_sat = cpy.getMinMaxSat();
    m_likelihood_thr = cpy.getLikeThr();
    m_threshold = cpy.getThr();
    m_threshold_mode = cpy.getThrMode();
    m_clustering_distance = cpy.getClustDist(); //NJL 10/AUG/2014
    m_clustering_min_pts = cpy.getClusteringMinPoints(); //NJL 13/FEB/2015
    m_has_max_radius = cpy.getHasMaxRad();
    m_has_auto_threshold = cpy.getHasAutoThr();
    m_has_hue_filter = cpy.getHasHueFilt();
    m_has_outlier_filter = cpy.getHasOutlierFilt();
    m_has_clustering_distance = cpy.getHasClustDist(); //NJL 10/AUG/2014

    cpy.m_gui_filter->copyTo(*m_gui_filter);
    cpy.m_image.copyTo(m_image);
    cpy.m_mask.copyTo(m_mask);
    return *this;
}
