#include "Predictor.hpp"
#include <stdlib.h>

#include "opencv2/highgui/highgui.hpp"//todel


#if CV_MAJOR_VERSION < 3
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
    //Reset random seed
    *(m_trees.get_rng())= cvRNG(4);
}

#else
Predictor::Predictor(){
    m_trees = cv::ml::RTrees::create();
    m_trees->setMaxDepth(10);
    m_trees->setMinSampleCount(10);
    m_trees->setRegressionAccuracy(0);
    m_trees->setUseSurrogates(false);
    m_trees->setMaxCategories(3);

    //m_trees->setPriors(0); //0
    m_trees->setCalculateVarImportance(true);
    m_trees->setActiveVarCount(false);



    m_trees->setTermCriteria({      cv::TermCriteria::MAX_ITER +
                                    cv::TermCriteria::EPS,
                                    100, 0.01
                                    });


    // *(m_trees.get_rng())= cvRNG(4);
}
#endif // CV_MAJOR_VERSION

void Predictor::loadTrainData(const std::string& str){
    #if CV_MAJOR_VERSION < 3
    m_trees.load(str.c_str());
    #else
    m_trees = cv::ml::StatModel::load<cv::ml::RTrees> (str);
    #endif // CV_MAJOR_VERSION
}

void Predictor::save(const std::string& str){
    #if CV_MAJOR_VERSION < 3
    m_trees.save(str.c_str());
    #else
    m_trees->save(str);
    #endif

}

void Predictor::train(const cv::Mat& features, const std::vector<signed char>& categs){
    cv::Mat categ_mat(categs.size(),1,CV_32S);
    for(int i = 0; i < categ_mat.rows;i++)
        categ_mat.at<long>(cv::Point(0,i)) = categs[i];
    #if CV_MAJOR_VERSION < 3
    m_trees.train(features,CV_ROW_SAMPLE,categ_mat,cv::Mat(),cv::Mat(),cv::Mat(),cv::Mat(),m_rt_params);
    #else
    m_trees->train(features, cv::ml::ROW_SAMPLE, categ_mat);
    #endif // CV_MAJOR_VERSION

}

void Predictor::predict(const cv::Mat& in, std::vector<signed char>& out){

    out.clear();
    out.reserve(in.rows);

    for(int i = 0; i < in.rows;i++){
        #if CV_MAJOR_VERSION < 3
        signed char cat = m_trees.predict(in.row(i));
        #else
        signed char cat = m_trees->predict(in.row(i));
        #endif
        out.push_back(cat) ;
    }
}

