#ifndef PREDICTOR_H
#define PREDICTOR_H

#define PREDIC_NOISE 0
#define PREDIC_SINGLE_COL 1
#define PREDIC_MULT_COL 2

#include "defines.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/ml/ml.hpp"

class Predictor
{
    public:
        Predictor();
        void train(const cv::Mat& features, const std::vector<signed char>& categs);
        void loadTrainData(const std::string& str);
        void save(const std::string& str);
        void predict(const cv::Mat& in, std::vector<signed char>& out);


    protected:
    private:
        //CV2
        //CvRTrees m_trees;
        //CvRTParams m_rt_params;
        //CV3
        cv::Ptr<cv::ml::RTrees> m_trees;
        // cv::ml::RTrees::create();
        //cv::ml::RTrees::Params m_trees;



};

#endif // PREDICTOR_H
