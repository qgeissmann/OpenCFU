#include "Step_FiltIPosition2D.hpp"

void Step_FiltIPosition2D::process(const void* src){
    const Result& in_numerical_result(*((Result*)(src)));
    if(!m_use_this_filter){
       m_step_numerical_result = in_numerical_result;
    }
    else{
      m_step_numerical_result = in_numerical_result;
      m_step_numerical_result.applyFilter(filter(in_numerical_result));
    }

    m_step_result = ((void*) &m_step_numerical_result);
}


void Step_FiltIPosition2D::updateParams(const void* src,bool was_forced){
    m_opts.getMask().getMat().copyTo(m_old_mask);
    makeROIPolygons();

}

bool Step_FiltIPosition2D::needReprocess(const void* src){
    if(!this->isSameImage(m_old_mask,m_opts.getMask().getMat()))
        return true;
    else
        return false;
}


void Step_FiltIPosition2D::makeROIPolygons(){
    cont_chunk tmp_chunk;
    if(!m_old_mask.empty()){



        cv::Mat tmp_img;

        unsigned int low,high;
        double a,b;
        cv::minMaxLoc(m_old_mask, &a, &b, NULL,NULL,m_old_mask);
        low = (int) a;
        high = (int) b;

        for(unsigned int i=low;i<high+1;++i){

            m_old_mask.copyTo(tmp_img);
            cv::inRange(m_old_mask, i,i,tmp_img);
            cont_chunk tmp_small_chunk;
            cv::findContours(tmp_img, tmp_small_chunk, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
            for(auto& c : tmp_small_chunk){
                tmp_chunk.push_back(c);
                m_ROIS_val.push_back(i);

            }
        }
        DEV_INFOS("found "<<tmp_chunk.size()<<"ROIs");
    }
    std::swap(tmp_chunk,m_ROIS_polygs);
}

std::vector<int> Step_FiltIPosition2D::filter(const Result& in_numerical_result){
    std::vector<int> valid(in_numerical_result.size(),1);
    if(!m_ROIS_polygs.empty()){
        for(unsigned int i = 0; i < valid.size(); i++){
            const OneObjectRow& oor = in_numerical_result.getRow(i);
            cv::Point2f center = (oor.getPoint(0) + oor.getPoint(2) ) * 0.5;

            int val=0;
            float how_far_from_edge = INT_MIN;

            for(unsigned int j = 0; j < m_ROIS_polygs.size(); ++j){
                float dist = cv::pointPolygonTest(m_ROIS_polygs[j], center, true);

                if(dist > 0){
                    if(dist > how_far_from_edge ){
                        how_far_from_edge = dist;
                        val = m_ROIS_val[j];
                    }
                }
            }
            if(val > 0 && how_far_from_edge > oor.getRadius() ){

                valid[i] = val;
            }
            else
                valid[i] = 0;
        }
    }
    return valid;
}

