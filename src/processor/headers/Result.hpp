#ifndef RESULT_H
#define RESULT_H

#include "defines.hpp"
#include <vector>
#include "ContourFamily.hpp"
#include "Features.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <unordered_map>
#include <string>

bool pairCompare(const std::pair< int, std::vector<double> >& a, const std::pair< int, std::vector<double> >& b);

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
        const cv::Scalar getLABMean() const{return m_LAB_mean;} //NJL 11/AUG/2014
        void setValid(bool b){m_valid = b;}
        void setGUIValid(signed char i){m_GUI_mask = i;}
        void setROI(int r){m_ROI = r;}

        const int getColorClusterID() const{return m_color_cluster_ID;} //NJL 11/AUG/2014
        void setColorClusterID(int color_cluster_ID){m_color_cluster_ID = color_cluster_ID;} //NJL 11/AUG/2014

        const cv::Scalar getClusterColor() const{return m_cluster_color;} //NJL 02/SEP/2014
        void setClusterColor(cv::Scalar cluster_colour){m_cluster_color = cluster_colour;} //NJL 02/SEP/2014

        std::string print() const ;
        static std::string printHeader();

    private:

        cv::RotatedRect m_rrect;
        cv::Scalar m_BGR_mean;
        cv::Scalar m_BGR_sd;
        cv::Scalar m_cluster_color; //NJL 02/SEP/2014
        cv::Scalar m_LAB_mean; //NJL 11/AUG/2014

        int m_sat_mean;
        int m_hue_mean;
        int m_n_in_clust;
        int m_area;
        int m_rad;
        bool m_valid;
        signed char m_GUI_mask; //-1 = neutral, 0 = force invalid, 1 = force valid
        int m_ROI;
        int cluster_class; //for unsupervised leaning
        int m_color_cluster_ID; //NJL 11/AUG/2014
};

/**
 *
 *
 *
 */
class ClusterData{
    public:
        ClusterData();
        ~ClusterData(){};
        void addCluster(int id, int pop, cv::Scalar col){
            if (!m_clusters.count(id)){
                m_clusters.emplace( id, std::make_pair(pop, col) );
            }
            else{
                DEV_INFOS("Duplicate cluster ID added");
                m_clusters.erase(id);
                m_clusters.emplace( id, std::make_pair(pop, col) );
                }
        };

        // Note that using this method only the first colour added is used. We only print to
        // DEV_INFOS if there is a mismatch
        void addCluster(int id, cv::Scalar col){
            if (!m_clusters.count(id)){
                m_clusters.emplace( id, std::make_pair(1, col) );
            }
            else{
                m_clusters.find(id)->second.first++;
                if (m_clusters.find(id)->second.second != col)
                    DEV_INFOS("Warning: Colour mismatch has occurred in cluster ID "<<id);
            }
        };

        void incrementClusterPop(int id){m_clusters.find(id)->second.first++;}
        const int clusterPop(int id) const{ return (m_clusters.count(id)) ? m_clusters.find(id)->second.first : 0;}
        const cv::Scalar clusterColor(int id) const{cv::Scalar cvscl(0,0,0); return (m_clusters.count(id)) ? m_clusters.find(id)->second.second : cvscl;}

        const int clustersTotal() const{return m_clusters.size();};
        const std::string str() const;


    private:
        std::unordered_map< int, std::pair<int, cv::Scalar> > m_clusters;
};


class ROIData{
    public:
        ROIData(){};

        const ClusterData& getROIClusterData(int roi) const{
            if (!m_roi_clusters.count(roi)){
                DEV_INFOS("Could not find roi");
                return m_empty_cluster_data;
            }
            else
                return (m_roi_clusters.find(roi)->second);
        }

        ClusterData& addROIClusterData(int roi) {
            if (!m_roi_clusters.count(roi)){
                ClusterData thisROI;
                m_roi_clusters.emplace(roi, thisROI);
            }
            return (m_roi_clusters.find(roi)->second);
        }

        const std::vector<int> getROIkeys() const{
            std::vector<int> keys;
            for (auto &it : m_roi_clusters){
                keys.push_back(it.first);
            }
            std::sort(keys.begin(),keys.end());
            return keys;
        }

        void delClusterData(int roi) { m_roi_clusters.erase(roi); }
        void clear() {m_roi_clusters.clear();}

        //void addPoint(OneObjectRow&);

    private:
        std::unordered_map<int, ClusterData> m_roi_clusters;
        ClusterData m_empty_cluster_data;

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
        void recluster(const std::vector< std::pair<int,int> > clustered); //NJL 13/AUG/2014
        void ColorProcessing(bool ordering); //NJL 02/SEP/2014, 13/FEB/2014
        void uncluster(); // NJL 01/SEP/2014

        //const ClusterData& getClusterData() const{return m_clusterData;} //NJL 03/SEP/2014 //deprecated by getROIData(0)
        const ClusterData& getROIClusterData(int roi) const{return m_roi_data.getROIClusterData(roi);} //NJL 10/SEP/2014
        const std::vector<int> getROIs() const{return m_roi_data.getROIkeys();}

    private:
        int m_n_valid;
        std::vector<OneObjectRow> v;
        bool m_same_objects;
        ClusterData m_clusterData;
        ROIData m_roi_data;


};



#endif // RESULT_H
