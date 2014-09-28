#include "Step_4.hpp"

/*
void testFun(std::vector<ContourFamily>& contour_fams, std::vector<signed char>& categ){
    unsigned int hi = contour_fams.size();

    for(unsigned int i=0; i < hi; ++i ){

        const cont_chunk& tmp_contours = (contour_fams[i]).contours;
        const hier_chunk& tmp_hiers = contour_fams[i].hierarchies;
        cv::Rect rect = cv::boundingRect(tmp_contours[0]);
        cv::Mat miniTmp(rect.height,rect.width,CV_8UC1,cv::Scalar(0));

        if(contour_fams[i].n_per_clust >1 ){

            std::stringstream ss;
            ss<<"/tmp/"<<categ[i]<<"_"<<contour_fams[i].n_per_clust<<"_"<<i<<".png";
            cv::drawContours(miniTmp,tmp_contours,-1,cv::Scalar(255),-1,8, tmp_hiers, 2, cv::Point(-rect.x,-rect.y));
            cv::imwrite(ss.str(),miniTmp);


        }
    }
}
*/
//montage S_* -background black -geometry +2+2 test.png



void Step_4::updateParams(const void* src,bool was_forced){
    m_min_radius = m_opts.getMinMaxRad().first;

    m_has_max_radius = m_opts.getHasMaxRad();
    m_max_cont_width = std::min(((cv::Mat*)src)->cols,((cv::Mat*)src)->rows) / 2;

    if(m_has_max_radius)
        m_max_radius = m_opts.getMinMaxRad().second;
    else
        m_max_radius = m_max_cont_width;

    m_has_auto_thr = m_opts.getHasAutoThr();
    m_threshold = m_opts.getThr();
}

bool Step_4::needReprocess(const void* src){

 if (!m_has_auto_thr){
      if(m_opts.getHasAutoThr())
        return true;
      if (m_threshold != m_opts.getThr())
        return true;
    }
    else{
        if(!m_opts.getHasAutoThr())
            return true;
    }
    return false;
}

void Step_4::process(const void* src){
    DEV_INFOS("Pass Two starting ...");

    const cv::Mat in = *((cv::Mat*)src);

    std::vector<ContourFamily> contour_fams;
    makeContourChunksVect(in,contour_fams);
    DEV_INFOS(contour_fams.size());
    cv::Mat feature_mat;
    preFilterContourSize(contour_fams);
    this->makeFeaturesMatrix(contour_fams,feature_mat);
    std::vector<signed char> categ;
    m_predictor.predict(feature_mat,categ);

    m_contour_spliter.split(contour_fams,categ);
    std::vector<ContourFamily> contour_fams_split, contour_fams_unsplit;

    separateUnsplited(contour_fams,contour_fams_unsplit,contour_fams_split);

    cv::Mat feature_mat_split, feature_mat_unsplit;
    std::vector<signed char> categ_split, categ_unsplit;

    preFilterContourSize(contour_fams_split);
    this->makeFeaturesMatrix(contour_fams_split,feature_mat_split);
    m_predictor_ps.predict(feature_mat_split,categ_split);


    preFilterContourSize(contour_fams_unsplit);
    this->makeFeaturesMatrix(contour_fams_unsplit,feature_mat_unsplit);
    m_predictor_ps.predict(feature_mat_unsplit,categ_unsplit);


    contour_fams_split.insert( contour_fams_split.end(), contour_fams_unsplit.begin(), contour_fams_unsplit.end() );
    std::swap(contour_fams_split,contour_fams);
    categ_split.insert( categ_split.end(), categ_unsplit.begin(), categ_unsplit.end() );
    std::swap(categ_split,categ);


    writeNumResults(contour_fams,categ);
    m_step_result = (void*) &m_step_numerical_result;

}

void Step_4::preFilterContourSize(std::vector<ContourFamily>& contour_fams){
    std::vector<ContourFamily> tmp_fams;
    tmp_fams.reserve(contour_fams.size());

    for(ContourFamily& i : contour_fams){
        if(i.contours[0].size() > 6){
            cv::Point p = Features::calculateWH(i.contours[0]);
            if(p.x > m_min_radius && p.y < m_max_radius)
                tmp_fams.push_back(i);
        }
    }
    std::swap(tmp_fams,contour_fams);
}


void Step_4::makeContourChunksVect(const cv::Mat& src,std::vector<ContourFamily>& contour_fams ){
    cv::Mat tmp_mat;
    cont_chunk coutours;
    hier_chunk hierachies;

    if(m_has_auto_thr)
        cv::threshold(src,tmp_mat,m_threshold,255,cv::THRESH_BINARY|cv::THRESH_OTSU);
    else
        cv::threshold(src,tmp_mat,m_threshold,255,CV_THRESH_BINARY);



    cv::findContours(tmp_mat, coutours,hierachies, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);
    contour_fams.reserve(coutours.size());
    unsigned int c=0;
    unsigned int CC = coutours.size();
    while ( c < CC){
        unsigned int nHoles = 0;
        if( hierachies[c][0] > 0)
            nHoles = hierachies[c][0]-c-1;
        else
            nHoles = CC - (c+1);
        contour_fams.push_back(ContourFamily(cont_chunk(coutours.begin()+c,coutours.begin()+c+nHoles+1)));
        c += nHoles+1;
    }
}


void Step_4::writeNumResults(const std::vector<ContourFamily>& contour_fams, const std::vector<signed char>& categ){

    std::vector<unsigned int> valid_idx;
    for(unsigned int i=0;i < categ.size() ; i++)
        if(categ[i] == 'S')
            valid_idx.push_back(i);

    DEV_INFOS(valid_idx.size());
    m_step_numerical_result.reset(valid_idx.size());

    for(unsigned int i=0;i < valid_idx.size() ; i++){
         unsigned int idx = valid_idx[i];
            m_step_numerical_result.add_at(OneObjectRow(contour_fams[idx],m_raw_img),i);
        }

}

void Step_4::separateUnsplited(const std::vector<ContourFamily>& contour_fams,std::vector<ContourFamily>& contour_fams_unsplit,std::vector<ContourFamily>& contour_fams_split){
    contour_fams_split.reserve(contour_fams.size());
    contour_fams_unsplit.reserve(contour_fams.size());
    for(auto& f : contour_fams){
        if(f.n_per_clust > 1)
            contour_fams_split.push_back(f);
        else
            contour_fams_unsplit.push_back(f);

    }
    DEV_INFOS("N Splitted"<<contour_fams_split.size());
    DEV_INFOS("N UnSplitted"<<contour_fams_unsplit.size());
}
