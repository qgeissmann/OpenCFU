#ifndef STEP_COLOURCLUSTER_H
#define STEP_COLOURCLUSTER_H

#include "defines.hpp"
#include "Step_BaseClass.hpp"
#include <math.h>
#include <algorithm>
#include <utility>
#include <unordered_map>
#include <string>
#include <vector>



class ClusterPoint
{
    public:
        //Constructor
        ClusterPoint(int id, int cluster_id, bool visited, cv::Scalar color);

        //Constant returns
        const cv::Scalar getColor()const{return m_color;}
        const int getID()const{return m_id;}

        //Variable returns and setters
        bool getVisited(){return m_visited;}
        int getClusterID(){return m_cluster_id;}

        void setVisited(bool visited){m_visited = visited;}
        void setClusterID(int cluster_id){m_cluster_id = cluster_id;}

    private:
        int m_id;
        int m_cluster_id;
        bool m_visited;
        cv::Scalar m_color;


};

class Step_ColourCluster : public Step_BaseClass
{
    public:
        Step_ColourCluster(ProcessingOptions& opts):
            Step_BaseClass(opts)
            {}


    protected:
        void process(const void* src);
        void updateParams(const void* src,bool was_forced);
        bool needReprocess(const void* src);
        std::vector< std::pair<int,int> >  cluster(const Result& in_numerical_result);
    private:
        bool m_use_this_filter;
        double m_clustering_distance;
        int m_min_pts;
        Result m_step_numerical_result;

        //relevant for header parameters for dbscan alorithm
        std::vector<ClusterPoint> m_cluster_vector;
        std::vector<ClusterPoint> m_neighbours;

        double m_clustering_distance_2;
        int m_min_cluster_pts;
        int m_current_cluster;

        void dbscan();
        std::vector<std::vector<ClusterPoint>::iterator> findNeighbours(ClusterPoint);
        void expandCluster(ClusterPoint);

        //comparator for sorting pairs, from stackexchange

};




#endif // STEP_COLOURCLUSTER_H
