#include "Result.hpp"


OneObjectRow::OneObjectRow():
m_n_in_clust(0),m_valid(false),m_ROI(1){}

OneObjectRow::OneObjectRow(ContourFamily cont_fam,const cv::Mat& raw_img):
    m_n_in_clust(cont_fam.n_per_clust),m_valid(true),m_ROI(1)
    {

        m_rrect = cv::minAreaRect(cont_fam.contours[0]);
        m_rad = Features::calculateWH(cont_fam.contours[0]).x;
        cv::Rect rect = cv::boundingRect(cont_fam.contours[0]);

        cv::Mat mini_mask(rect.height,rect.width,CV_8UC1,cv::Scalar(0));
        cv::Mat mini_img;
        raw_img(rect).copyTo(mini_img);
        cv::drawContours(mini_mask,cont_fam.contours,0,cv::Scalar(255),-1,8, cont_fam.hierarchies, 2, cv::Point(-rect.x,-rect.y));

        cv::Mat mean, stddev;
        cv::meanStdDev(mini_img, mean, stddev, mini_mask);
        m_area = cv::countNonZero(mini_mask);
        stddev +=1;

        m_BGR_mean = mean.at<cv::Scalar>(0,0);
        m_BGR_sd = stddev.at<cv::Scalar>(0,0);



        cv::Mat one_pix(1,1,CV_8UC3,m_BGR_mean);
        cv::cvtColor(one_pix,one_pix,CV_BGR2HLS);
        cv::Scalar mean_sc = cv::mean(one_pix); //?

        m_hue_mean =((int) mean_sc [0])*2;
        m_sat_mean = (int) mean_sc [2];



    }



void Result::reset(size_t s){
    v.clear();
    v.resize(s);
    m_n_valid = 0;
}

void Result::add_at(const OneObjectRow& row,const unsigned int i){
    v[i] = row;
    if(row.isValid())
        m_n_valid++;
}


 void Result::applyFilter(const std::vector<bool>& valid){
    if (valid.empty())
        return;

    assert(valid.size() == v.size());

    m_n_valid = 0;
    for(unsigned int i = 0; i != valid.size(); ++i){
        int test = (int)v[i].getGUIValid();
        if(test > 0){
            v[i].setValid(true);
            ++m_n_valid;
        }

        else if(test == -1){
            if(!valid[i])
               v[i].setValid(false);
            else{
               ++m_n_valid;
               v[i].setValid(true);
            }
        }

        else if(test == 0)
            v[i].setValid(false);
    }
}

void Result::applyFilter(const std::vector<int>& valid){

    if (valid.empty())
        return;

    assert(valid.size() == v.size());

    m_n_valid = 0;
    for(unsigned int i = 0; i != valid.size(); ++i){
        int test = (int)v[i].getGUIValid();
        if(test > 0){
            v[i].setValid(true);
            ++m_n_valid;
           ////MODIF FOR ROI ATTRIBUTION
           v[i].setROI(valid[i]);
        }

        else if(test == -1){
            if(!valid[i]){
               v[i].setValid(false);
               v[i].setROI(0);
            }
            else{
               ++m_n_valid;
               v[i].setValid(true);
               ////MODIF FOR ROI ATTRIBUTION
               v[i].setROI(valid[i]);
            }
        }

        else if(test == 0)
            v[i].setValid(false);
    }
}


void Result::applyGuiFilter(const cv::Mat& valid){
    if (valid.empty()){
        DEV_INFOS("Empty filter!");
        return;
    }

    std::vector<bool> boo(valid.rows);

    m_n_valid = 0;
    unsigned int nrow = (unsigned)valid.rows;
    for(unsigned int i = 0; i < nrow; i++){
        signed char val = valid.at<signed char>(i,0);
        v[i].setGUIValid(val);
        boo[i] = true;
    }

    applyFilter(boo);

}

