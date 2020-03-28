#ifndef MASKROI_H
#define MASKROI_H

#include "defines.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

class MaskROI
{
    public:
        MaskROI(int pretype):type(pretype){
        }

        MaskROI(const std::string& path){
            cv::imread(path,cv::IMREAD_GRAYSCALE).copyTo(m_original_mat);
            type = MASK_TYPE_FILE;
        }
        MaskROI(cv::Mat img){
            img.copyTo(m_original_mat);
            type = MASK_TYPE_FILE;
        }
        MaskROI(cv::Mat img,int typepp){

            if(typepp != MASK_TYPE_NONE)
                img.copyTo(m_original_mat);
                img.copyTo(m_mat);
            type = typepp;
        }

        MaskROI():type(MASK_TYPE_NONE){};
        void setFromPoints(const std::vector< std::pair<std::vector<cv::Point2f>,int > >& points,int width,int height);
        void none(){
            type = MASK_TYPE_NONE;
            m_original_mat = cv::Mat();
            }

        const bool isValid()const {
            if(type == MASK_TYPE_NONE || type == MASK_TYPE_AUTO)
                return true;
            else if(type == MASK_TYPE_FILE && !m_original_mat.empty())
                return true;
            else if(type == MASK_TYPE_DRAW && !m_original_mat.empty())
                return true;

            else
                return false;
        }
        void copyTo(MaskROI& cpy)const {
            cpy = MaskROI(m_original_mat,type);
            if(type != MASK_TYPE_AUTO)
                cpy.update(m_mat);
        }
        const cv::Mat& getMat()const{
                DEV_INFOS("??");
                return m_mat;

            }

        int type;
        void update(const cv::Mat parent_image);
    protected:
        std::vector<float> circleFrom3(const std::vector<cv::Point2f>& points);
        void makeAutoMask(const cv::Mat& parent);
    private:

        cv::Mat m_mat;
        cv::Mat m_original_mat;
};

#endif // MASKROI_H
