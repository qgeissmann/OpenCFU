#include "Step_FiltLik.hpp"

void Step_FiltLik::updateParams(const void* src,bool was_forced){
    m_use_this_filter = m_opts.getHasOutlierFilt();
    m_lik_thr = m_opts.getLikeThr();
}

bool Step_FiltLik::needReprocess(const void* src){
    bool toReprocess = false;
    toReprocess = ( m_use_this_filter != m_opts.getHasOutlierFilt()||
                     m_lik_thr != m_opts.getLikeThr());
    DEV_INFOS("reprocess?"<<(int)toReprocess);
    return toReprocess;
}

void Step_FiltLik::process(const void* src){
    const Result& in_numerical_result(*((Result*)(src)));
    if(!m_use_this_filter){
       m_step_numerical_result = in_numerical_result;
       DEV_INFOS("Do not filter");
    }
    else{
      m_step_numerical_result = in_numerical_result;
      m_step_numerical_result.applyFilter(filter(in_numerical_result));
      DEV_INFOS("filtering");
    }
    m_step_result = ((void*) &m_step_numerical_result);
}

std::vector<bool>  Step_FiltLik::filter(const Result& in_numerical_result){
    std::vector<int> idx_of_good_objects;

    for(unsigned int i = 0; i < in_numerical_result.size(); i++){
        const OneObjectRow& oor = in_numerical_result.getRow(i);
        if(oor.isValid() && oor.getNInClust() == 1){
          idx_of_good_objects.push_back(i);
        }
    }

    DEV_INFOS("N Good objects = "<<idx_of_good_objects.size());

    if(idx_of_good_objects.size() < 6 ){
         m_step_numerical_result = in_numerical_result;
         std::vector<bool> r;
        return r;
    }
    int j =0;
    cv::Mat features(idx_of_good_objects.size(),N_FEATURES,CV_32F);
    for(int i : idx_of_good_objects){
        const OneObjectRow& oor = in_numerical_result.getRow(i);
        if(oor.isValid() && oor.getNInClust() == 1){
            cv::Mat one_row = features.row(j);
            one_row.at<float>(0,0) = oor.getBGRMean()[0];
            one_row.at<float>(0,1) = oor.getBGRMean()[1];
            one_row.at<float>(0,2) = oor.getBGRMean()[2];
            one_row.at<float>(0,3) = oor.getBGRSd()[0];
            one_row.at<float>(0,4) = oor.getBGRSd()[1];
            one_row.at<float>(0,5) = oor.getBGRSd()[2];
            j++;
        }

    }

    for(int i = 0; i < features.cols;i++){
        cv::Mat mean, stddev;
        cv::meanStdDev(features.col(i), mean, stddev);
        m_means[i] = mean.at<double>(0,0);
        m_sds[i] = stddev.at<double>(0,0);
    }


    std::vector<bool> valid(in_numerical_result.size(),false);
    for(unsigned int i = 0; i < valid.size(); i++){
        const OneObjectRow& oor = in_numerical_result.getRow(i);
        if(oor.isValid()){
            std::vector<float> feats;
            feats.reserve(6);
            feats.push_back(oor.getBGRMean()[0]);
            feats.push_back(oor.getBGRMean()[1]);
            feats.push_back(oor.getBGRMean()[2]);
            feats.push_back(oor.getBGRSd()[0]);
            feats.push_back(oor.getBGRSd()[1]);
            feats.push_back(oor.getBGRSd()[2]);

            float score = this->calcLikScore(feats);


            if(score > m_lik_thr)
                valid[i] = false;
            else
                valid[i] = true;
        }
    }
    return(valid);
}


float Step_FiltLik::calcLikScore(std::vector<float>& in){
    float p =0;
    float pi = 3.14;
    for(int i = 0; i < N_FEATURES;i++){
        double sqrt_dif_by_sd = -std::pow((m_means[i] - in[i] )/m_sds[i],2)/2;
        double A = std::exp(sqrt_dif_by_sd);
        double B = m_sds[i] * std::sqrt(2*pi);
        double pp = A / B;
        p += std::log(pp);
    }
    return -1 * p;
}
