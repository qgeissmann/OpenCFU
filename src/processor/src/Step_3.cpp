#include "Step_3.hpp"

void Step_3::updateParams(const void* src,bool was_forced){
    m_min_radius = m_opts.getMinMaxRad().first;

    m_has_max_radius = m_opts.getHasMaxRad();
    m_max_cont_width = std::min(((cv::Mat*)src)->cols,((cv::Mat*)src)->rows) / 2;
    if(m_has_max_radius)
        m_max_radius = m_opts.getMinMaxRad().second;
    else
        m_max_radius = m_max_cont_width;
}

bool Step_3::needReprocess(const void* src){
    if (m_has_max_radius){
      if(!m_opts.getHasMaxRad())
        return true;
      if (m_max_radius != m_opts.getMinMaxRad().second)
        return true;
    }
    else{
        if(m_opts.getHasMaxRad())
            return true;
    }
    if(m_min_radius != m_opts.getMinMaxRad().first)
        return true;
    return false;
}

void Step_3::process(const void* src){
    DEV_INFOS("Pass One starting ...");
    cv::Mat feature_mat;
    std::vector<ContourFamily> contour_fams;
    const cv::Mat in = *((cv::Mat*)src);
    std::vector<signed char> categ;
    m_step_img = cv::Mat(in.rows,in.cols,CV_8U,cv::Scalar(0));
    this->makeContourChunksVect(in,contour_fams);
//    this->preFilterContourSize(contour_fams);
    this->makeFeaturesMatrix(contour_fams,feature_mat);
    m_predictor.predict(feature_mat,categ);
    this->drawAllValid(m_step_img ,contour_fams,categ);
    m_step_result = (void*) &m_step_img;
    DEV_INFOS("Pass One finished ...");
}



bool Step_3::isSizeOK(const std::vector<cv::Point>& cont){

    if(cont.size() < 7)
        return false;
    cv::Point p = Features::calculateWH(cont);
    if(p.x < m_min_radius || p.y > m_max_radius)
        return false;

    return true;
}


void Step_3::makeFeaturesMatrix(const std::vector<ContourFamily>& contour_fams,cv::Mat& dst){
    unsigned int n = contour_fams.size();
    unsigned int n_features = m_featureMaker.getNFeature();
    dst = cv::Mat(n,n_features,CV_32F);

    //#pragma omp parallel for schedule(guided)
    for(unsigned int i = 0; i < n;i++){

        cv::Mat tmp_row(1,n_features,CV_32F);
        m_featureMaker.calcFeatures(contour_fams[i],tmp_row);
        //#pragma omp critical
        {
        tmp_row.copyTo(dst.row(i));
        //DEV_INFOS(i);
        }
    }
}

void Step_3::makeContourChunksVect(const cv::Mat& src,std::vector<ContourFamily>& contour_fams ){
    unsigned int low,high;
    double a,b;
    cv::minMaxLoc(src, &a, &b, NULL,NULL);
    low = (int) a;
    high = (int) b;

    assert (low<=high);
    unsigned int lim = high - low;
    std::vector<cont_chunk> all_contours_chunk(high-low);
    std::vector<hier_chunk> all_hieras_chunk(high-low);

    #pragma omp parallel for schedule(dynamic)
    for(unsigned int i = 2; i < lim; i+=2){
        cv::Mat thrd;
        cv::threshold(src,thrd,i+low,255,cv::THRESH_BINARY);
        cv::findContours(thrd, all_contours_chunk[i],all_hieras_chunk[i], cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);

        for(auto& c : all_contours_chunk[i] ){
            if(c.size()>100)
                subsample(c,c,100);
        }
    }

    #pragma omp parallel for schedule(dynamic)
    for(unsigned int i = 0; i < lim; i++){
        cont_chunk& chunk = all_contours_chunk[i];
        hier_chunk& hierarchy = all_hieras_chunk[i];
        unsigned int c=0;
        unsigned int CC = chunk.size();
        while ( c < CC){
            /*if this is not the lastest non-hole*/
            unsigned int nHoles = 0;
            if( hierarchy[c][0] > 0)
                nHoles = hierarchy[c][0]-c-1;
            else
                nHoles = CC - (c+1);

            if(isSizeOK(chunk[c])){
                #pragma omp critical
                {
                contour_fams.push_back(ContourFamily(cont_chunk(chunk.begin()+c,chunk.begin()+c+nHoles+1)));
                }
            }
            c += nHoles+1;
        }
    }
}
void Step_3::drawAllValid(cv::Mat& inout,std::vector<ContourFamily>& contour_fams, std::vector<signed char>& categ){
    unsigned int hi = contour_fams.size();
    #pragma omp parallel for schedule(dynamic)
    for(unsigned int i=0; i < hi; ++i ){
        const cont_chunk& tmp_contours = (contour_fams[i]).contours;
        const hier_chunk& tmp_hiers = contour_fams[i].hierarchies;
        cv::Rect rect = cv::boundingRect(tmp_contours[0]);
        cv::Mat miniTmp(rect.height,rect.width,CV_8UC1,cv::Scalar(0));
        if(categ[i] != 'N'){
            cv::drawContours(miniTmp,tmp_contours,-1,cv::Scalar(1),-1,8, tmp_hiers, 2, cv::Point(-rect.x,-rect.y));
            #pragma omp critical
            {inout(rect) = miniTmp + inout(rect);}
        }
    }
}


void  Step_3::subsample(const std::vector<cv::Point>& in, std::vector<cv::Point>& out,const unsigned int size_out){

    std::vector<cv::Point> preout (size_out);
    unsigned int size_in = in.size();
    std::vector<float> data((size_in+1)*2);
    for(unsigned int i = 0; i != size_in; i++){
        const cv::Point& p = in[i];
        data[i*2] = p.x;
        data[i*2+1] = p.y;
    }
    data[size_in*2] = data[0];
    data[size_in*2+1] = data[1];
    cv::Mat tmp_mat = cv::Mat(data,false).reshape(1,size_in+1).t();
    cv::resize(tmp_mat,tmp_mat,cv::Size(size_out+1,2),cv::INTER_LINEAR);
    for(unsigned int i = 0;i != size_out; ++i){
        int x = std::round(tmp_mat.at<float>(0,i));
        int y = std::round(tmp_mat.at<float>(1,i));
        preout[i] = cv::Point(x,y);
    }
    std::swap(preout,out);
}

