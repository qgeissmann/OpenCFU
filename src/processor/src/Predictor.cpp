#include "Predictor.hpp"


#include "opencv2/highgui/highgui.hpp"//todel

Predictor::Predictor():
m_rt_params(
10,//maxdepth
10,//mincount
0,//regression accur
false,//surogate?
3,//max categ
0,//pointer to priors
true,// var importance?
false,//nact_var
100,//max num tree
0.01,//accuracy
CV_TERMCRIT_EPS | CV_TERMCRIT_ITER//terminaison criteria
)
{
    /*Reset random seed*/
    *(m_trees.get_rng())= cvRNG(4);
}

void Predictor::loadTrainData(const std::string& str){
    m_trees.load(str.c_str());
}

void Predictor::save(const std::string& str){
    m_trees.save(str.c_str());
}

void Predictor::train(const cv::Mat& features, const std::vector<signed char>& categs){
    cv::Mat categ_mat(categs.size(),1,CV_32S);
    for(int i = 0; i < categ_mat.rows;i++)
        categ_mat.at<long>(cv::Point(0,i)) = categs[i];
    m_trees.train(features,CV_ROW_SAMPLE,categ_mat,cv::Mat(),cv::Mat(),cv::Mat(),cv::Mat(),m_rt_params);
    DEV_INFOS("Variable Importance:\n"<<m_trees.getVarImportance()<<std::endl);
}

void Predictor::predict(const cv::Mat& in, std::vector<signed char>& out){
    out.clear();
    out.reserve(in.rows);
    for(int i = 0; i < in.rows;i++){
        signed char cat = m_trees.predict(in.row(i));
        out.push_back(cat) ;
    }
}

