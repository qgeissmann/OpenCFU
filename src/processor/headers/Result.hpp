#ifndef RESULT_H
#define RESULT_H

#include "defines.hpp"
#include <vector>
#include "ContourFamily.hpp"
#include "Features.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class OneObjectRow{
    public:
        OneObjectRow();
        OneObjectRow(ContourFamily cont_fam,const cv::Mat& raw_img);
        const cv::Point2f getPoint(const int i) const{
            cv::Point2f points[4];
            m_rrect.points(points);
            return points[i];
            }
        const cv::RotatedRect& getRrect() const{
            return m_rrect;
        }
//        const ContourFamily& getContourFam() const{return m_cont_fam;}
//        void setPoints(const int i,const cv::Point2f p){ m_points[i] = p;}
        const bool isValid() const{return m_valid;}
        const int getROI() const{return m_ROI;}
        const signed char  getGUIValid() const {return m_GUI_mask;}
        const int getNInClust() const{return m_n_in_clust;}
        const int getArea() const{return m_area;}
        const int getRadius() const{return m_rad;}
        const int getHue() const{return m_hue_mean;}
        const int getSat() const{return m_sat_mean;}
        const cv::Scalar getBGRMean() const{return m_BGR_mean;}
        const cv::Scalar getBGRSd() const{return m_BGR_sd;}
        void setValid(bool b){m_valid = b;}
        void setGUIValid(signed char i){m_GUI_mask = i;}
        void setROI(int r){m_ROI = r;}
        std::string print() const ;
        std::string printHeader() const ;

    private:

        cv::RotatedRect m_rrect;
        cv::Scalar m_BGR_mean;
        cv::Scalar m_BGR_sd;
        int m_sat_mean;
        int m_hue_mean;
        int m_n_in_clust;
        int m_area;
        int m_rad;
        bool m_valid;
        signed char m_GUI_mask; //-1 = neutral, 0 = force invalid, 1 = force valid
        int m_ROI;
};

class Result{
    public:
        Result():m_same_objects(false){};

        const size_t size() const{return v.size();}
        void add_at(const OneObjectRow& row, const unsigned int i);

        void reset(size_t s);

        int getNValid() const{return m_n_valid;}

        const OneObjectRow& getRow(unsigned int i) const{ return v[i];}
        void applyFilter(const std::vector<bool>& valid);
        void applyFilter(const std::vector<int>& valid);
        void applyGuiFilter(const cv::Mat& valid);
        void setSameObjects(bool b){m_same_objects = b;}
        const bool getSameObjects()const {return m_same_objects;}
    private:
        int m_n_valid;
        std::vector<OneObjectRow> v;
        bool m_same_objects;
};

#endif // RESULT_H
