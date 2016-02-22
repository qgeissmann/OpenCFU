#include "Predictor.hpp"


#include "opencv2/highgui/highgui.hpp"//todel

//CV2
/*
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
    //CV2
    // *(m_trees.get_rng())= cvRNG(4);
}
*/



//CV3

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
                                    100, 0.1
                                    });


    // *(m_trees.get_rng())= cvRNG(4);
}

void Predictor::loadTrainData(const std::string& str){
    //CV2
    //m_trees.load(str.c_str())
    //CV3
    m_trees = cv::ml::StatModel::load<cv::ml::RTrees> ("/home/quentin/comput/opencfu-git/src/classifier/trainnedClassifier.xml");
}

void Predictor::save(const std::string& str){
    //CV2
    //m_trees.save(str.c_str());
    //CV3
    auto fs =  cv::FileStorage(str, cv::FileStorage::WRITE);
    m_trees->write(fs);

}

void Predictor::train(const cv::Mat& features, const std::vector<signed char>& categs){
    cv::Mat categ_mat(categs.size(),1,CV_32S);
    for(int i = 0; i < categ_mat.rows;i++)
        categ_mat.at<long>(cv::Point(0,i)) = categs[i];
    //CV2
    //m_trees.train(features,CV_ROW_SAMPLE,categ_mat,cv::Mat(),cv::Mat(),cv::Mat(),cv::Mat(),m_rt_params);

    //CV3
    //cv::Ptr<cv::ml::TrainData> data = cv::ml::TrainData::create(features, cv::ml::ROW_SAMPLE, categ_mat);
    //m_trees->train(features, cv::ml::ROW_SAMPLE, categ_mat);

    //std::cout<<"\nConfusion Matrix = \n REAL(top) -> PRED(left) \n"<<cv::format(categs,cv::Formatter::FMT_CSV)<<"\n"<<std::endl;

    m_trees->train(features, cv::ml::ROW_SAMPLE, categ_mat);


    //DEV_INFOS("Variable Importance:\n"<<m_trees.getVarImportance()<<std::endl);

    //CV2
    //DEV_INFOS("Variable Importance:\n"<<m_trees.getVarImportance()<<std::endl);

    //CV3
    //DEV_INFOS("Variable Importance:\n"<<m_trees->getVarImportance()<<std::endl);

}

void Predictor::predict(const cv::Mat& in, std::vector<signed char>& out){

    out.clear();
    out.reserve(in.rows);

    //cv::FileStorage read("/home/quentin/comput/opencfu-git/src/classifier/trainnedClassifier.xml", cv::FileStorage::READ);
    //DEV_INFOS("b");



    DEV_INFOS("a");
    for(int i = 0; i < in.rows;i++){
        //CV2
        //signed char cat = m_trees.predict(in.row(i));
        //CV3
        signed char cat = m_trees->predict(in.row(i));

        out.push_back(cat) ;
    }
}

