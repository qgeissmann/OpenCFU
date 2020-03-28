#ifndef PROCESSINGOPTIONS_H
#define PROCESSINGOPTIONS_H

#include "defines.hpp"
#include <memory>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "MaskROI.hpp"

class ProcessingOptions
{
    public:
/** \brief The default constructor
 */
        ProcessingOptions();
        ProcessingOptions& operator= (const ProcessingOptions& cpy);

/** \brief Getter for the m_min_max_radius variable
 * \return the minimal and maximal radii as a pair of integer
 */
        const std::pair<int,int> getMinMaxRad() const { return m_min_max_radius;}

/** \brief Getter for the m_cent_tol_hue variable
 * \return the center and tolerance for the hue filter as a pair of integer
 */
        const std::pair<int,int> getCenTolHue()const{return m_cent_tol_hue;}

/** \brief Getter for the m_cent_tol_sat variable
 * \return the center and tolerance for the saturation filter as a pair of integer
 */
        const std::pair<int,int> getMinMaxSat()const{return m_min_max_sat;}

/** \brief Getter for the m_likelihood_thr variable
 * \return the likelihood threshold as a double
 */
       const  double getLikeThr()const{return m_likelihood_thr;}

/** \brief Getter for the m_threshold variable
 * \return the threshold as an integer
 */
        const int getThr()const{return m_threshold;}

/** \brief Getter for the m_threshold_mode variable
 * \return the threshold mode as an integer
 */
        const int getThrMode()const{return m_threshold_mode;}

/** \brief Getter for the m_has_max_radius variable
 * \return the threshold as a bool
 */
        const bool getHasMaxRad()const{return m_has_max_radius;}

/** \brief Getter for the m_has_auto_threshold variable
 * \return the threshold as a bool
 */
        const bool getHasAutoThr()const{return m_has_auto_threshold;}

/** \brief Getter for the m_has_hue_filter variable
 * \return the threshold as a bool
 */
        const bool getHasHueFilt()const{return m_has_hue_filter;}

/** \brief Getter for the m_has_outlier_filter variable
 * \return the threshold as a bool
 */
        const bool getHasOutlierFilt()const{return m_has_outlier_filter;}

/** \brief Getter for the image
 * \return a ref to m_image as a cv::Mat
 */
       const cv::Mat& getImage()const{return m_image;}

/** \brief Getter for the image name
 * \return a string
 */
        const std::string& getImageName()const{return m_image_path;}


/** \brief Getter for the mask
 * \return a ref to m_mask as a cv::Mat
 */
        const MaskROI& getMask()const{return m_mask;}


        const cv::Mat& getGUIFilter()const {
            return *m_gui_filter;
            }

//NJL 10/AUG/2014
/** \brief Getter for the clustering distance
 *  \return double clustering search distance in L*a*b* colour space
 */

        const double getClustDist()const{return m_clustering_distance;}

//NJL 13/FEB/2015
/** \brief Getter for the clustering distance
 *  \return  int clustering_min_pts Number of neighbour points necessary to be in a cluster
 */

        const double getClusteringMinPoints()const{return m_clustering_min_pts;}

 //NJL 10/AUG/2014
/** \brief Getter for the has_clustering_distance variable
 *  \return bool state of the has_clustering_distance variable
 */
        const bool getHasClustDist()const{return m_has_clustering_distance;}


/** \brief Setter for m_image
 * \param str the name of the file to read the image from
 */
        bool setImage(const std::string str){
            cv::Mat tmpImg = cv::imread(str, cv::IMREAD_ANYDEPTH | cv::IMREAD_COLOR);

            //patch for 16bit depth imgs:
            if(tmpImg.depth() == CV_16U){
                double min, max;
                cv::minMaxLoc(tmpImg, &min, &max);
                double rat = max / 256.0;
                cv::divide(tmpImg,cv::Scalar(rat,rat,rat),tmpImg,1,CV_8UC3) ;//* (double)(1/2);
            }

            if(!tmpImg.empty()){
                m_image = tmpImg;
                m_image_path = str;
                DEV_INFOS("");
                m_mask.update(m_image);
                return true;
                }
            else
                return false;
            }
/** \brief Setter for m_image
 * \param src an image
 */
        bool setImage(const cv::Mat src){
            cv::Mat tmpImg;
            src.copyTo(tmpImg);
            if(!tmpImg.empty()){
                m_image = tmpImg;
                m_image_path = "img_from_memory";
                DEV_INFOS("");
                m_mask.update(m_image);
                return true;
                }
            else
                return false;
            }

/** \brief Setter for m_mask
 * \param src an image
 */
        bool setMask(const MaskROI& mask){

            if(mask.isValid()){

                mask.copyTo(m_mask);
                DEV_INFOS(m_image.channels());
                m_mask.update(m_image);
                return true;
            }
            else
                return false;

        }

/** \brief Setter for m_min_max_radius
 * \param min_max_radius the new value for m_min_max_radius
 */
        bool setMinMaxRad(const std::pair<int,int> min_max_radius){
            bool maxRadTest(true);
            if(m_has_max_radius)
                maxRadTest = (min_max_radius.first <= min_max_radius.second && min_max_radius.second >=0);
            if(min_max_radius.first >=0 && maxRadTest){
                m_min_max_radius = min_max_radius;
                return true;
                }
            else
                return false;
            }

/** \brief Setter for m_cent_tol_hue
 * \param v the new value for m_cent_tol_hue
 */
        bool setCenTolHue(const std::pair<int,int> cent_tol_hue){
            if( cent_tol_hue.first <= 360 && cent_tol_hue.first >= 0 &&
                cent_tol_hue.second <= 180 && cent_tol_hue.second>= 0){
                m_cent_tol_hue = cent_tol_hue;
                return true;
            }
            else
                return false;

            }
/** \brief Setter for m_min_max_radius
 * \param min_max_radius the new value for m_min_max_radius
 */
        bool setMinMaxSat(const std::pair<int,int> min_max_sat){
            bool maxSatTest(true);
            if(m_has_hue_filter)
                maxSatTest = (min_max_sat.first <= min_max_sat.second && min_max_sat.second >=0);
            if(min_max_sat.first >=0 && min_max_sat.second <256 && maxSatTest){
                m_min_max_sat = min_max_sat;
                return true;
                }
            else
                return false;
            }
/** \brief Setter for m_likelihood_thr
 * \param likelihood_thr the new value for m_likelihood_thr
 */
        bool setLikeThr(const double likelihood_thr){
            m_likelihood_thr = likelihood_thr;
            return true;
            }

/** \brief Setter for m_threshold
 * \param threshold the new value for m_threshold
 */
        bool setThr(const int threshold){
            if(threshold>=0 && threshold < 256){
                m_threshold = threshold;
                return true;
            }
            else
                return false;
        }

/** \brief Setter for m_threshold_mode
 * \param mode the new value for m_threshold_mode
 */
        bool setThrMode(const int mode){
            if(mode == OCFU_THR_NORM ||
                mode == OCFU_THR_INV ||
                mode == OCFU_THR_BILAT ){
                m_threshold_mode = mode;
                return true;
            }
            else
                return false;
        }

/** \brief Setter for m_has_max_radius
 * \param has_max_radius the new value for m_has_max_radius
 */
        void setHasMaxRad(const bool has_max_radius){m_has_max_radius = has_max_radius;}

/** \brief Setter for m_has_auto_threshold
 * \param has_auto_threshold the new value for m_has_auto_threshold
 */
        void setHasAutoThr(const bool has_auto_threshold){m_has_auto_threshold = has_auto_threshold;}

/** \brief Setter for m_hue_filter
 * \param has_hue_filter the new value for m_has_hue_filter
 */
        void setHasHueFilt(const bool has_hue_filter ){m_has_hue_filter = has_hue_filter;}

/** \brief Setter for m_has_outlier_filter
 * \param has_outlier_filter the new value for m_has_outlier_filter
 */
        void setHasOutlierFilt(const bool has_outlier_filter){
            m_has_outlier_filter = has_outlier_filter;
            }

        void setGUIFilter(std::shared_ptr<cv::Mat>& filt){
            m_gui_filter = filt;
            }

//NJL 14/AUG/2014
/** \brief Setter for Clustering Distance
 *  \param double clustering_distance clustering distance in L*a*b* color space
 */
        bool setClustDist(const double clustering_distance){
            if (clustering_distance>=0.1 && clustering_distance<=50.){
                m_clustering_distance = clustering_distance;
                return true;}
            else{
                return false;
            }
        }

//NJL 13/FEB/2015
/** \brief Setter for Clustering minimum points
 *  \param int clustering_min_pts Number of neighbour points necessary to be in a cluster
 */
        bool setClustMinPoints(const double clustering_min_pts){
            if (clustering_min_pts>=4 && clustering_min_pts<=50.){
                m_clustering_min_pts = clustering_min_pts;
                return true;}
            else{
                return false;
            }
        }

//NJL 14/AUG/2014
/** \brief Setter for m_has_clustering_distance
 *  \param bool has_clustering_distance state for whether to perform clustering
 */
        void setHasClustDist(const bool has_clustering_distance){m_has_clustering_distance = has_clustering_distance;}


    protected:
//        void ajustmaskToImg();
    private:
        cv::Mat m_image;
        std::string m_image_path;
        MaskROI m_mask;
        std::pair<int,int> m_min_max_radius;
        std::pair<int,int> m_cent_tol_hue;
        std::pair<int,int> m_min_max_sat;
        double m_likelihood_thr;
        double m_clustering_distance; //NJL 10/AUG/2014
        int m_clustering_min_pts; //NJL 13/FEB/2015
        int m_threshold;
        int m_threshold_mode;
        bool m_has_max_radius;
        bool m_has_auto_threshold;
        bool m_has_hue_filter;
        bool m_has_outlier_filter;
        bool m_has_clustering_distance; //NJL 10/AUG/2014

        std::shared_ptr<cv::Mat> m_gui_filter;
};

#endif // PROCESSINGOPTIONS_H
