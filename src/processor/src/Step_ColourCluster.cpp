#include "Step_ColourCluster.hpp"

ClusterPoint::ClusterPoint(int id, int cluster_id, bool visited, cv::Scalar color){
    m_id = id;
    m_cluster_id = cluster_id;
    m_visited = visited;
    m_color = color;
}

void Step_ColourCluster::process(const void* src){
    const Result& in_numerical_result(*((Result*)(src)));
    if(!m_use_this_filter){
        m_step_numerical_result = in_numerical_result;
        DEV_INFOS("Unclustering");
        m_step_numerical_result.uncluster();
    }
    else{
        DEV_INFOS("Reclustering");
        m_step_numerical_result = in_numerical_result;
        m_step_numerical_result.uncluster();
        m_step_numerical_result.recluster(cluster(in_numerical_result));
    }
    m_step_result = ((void*) &m_step_numerical_result);
}


void Step_ColourCluster::updateParams(const void* src,bool was_forced){
    m_use_this_filter = m_opts.getHasClustDist();
    m_clustering_distance = m_opts.getClustDist();
    m_clustering_distance_2 = m_clustering_distance*m_clustering_distance;
    DEV_INFOS("Cluster Distance set to "<<m_clustering_distance);

}

bool Step_ColourCluster::needReprocess(const void* src){
    bool toReprocess = false;
    toReprocess = ( m_use_this_filter != m_opts.getHasClustDist() ||
                     m_clustering_distance != m_opts.getClustDist() );
    return toReprocess;
}


/**
 *
 */
std::vector< std::pair<int,int> > Step_ColourCluster::cluster(const Result& in_numerical_result){


    std::vector< std::pair<int,int> > result;
    m_cluster_vector.clear();
    for(unsigned int ii = 0; ii < in_numerical_result.size(); ii++){
        const OneObjectRow& oor = in_numerical_result.getRow(ii);
        //populate the list of valid items with LAB color mean, ID and the cluster state
        if ( oor.isValid() ){
            m_cluster_vector.push_back(ClusterPoint( ii, 0, false, oor.getLABMean() ));
        }
        else {
            result.push_back( std::make_pair<int,int>( ii, 0 ) );
        }
    }

    dbscan();

    for (std::vector<ClusterPoint>::iterator it = m_cluster_vector.begin(); it != m_cluster_vector.end(); ++it){
        result.push_back(std::make_pair<int,int>( it->getID(), it->getClusterID() ));
    }


    return result;
}

/**
 *
 */
void Step_ColourCluster::dbscan(){
    DEV_INFOS("Launching density scanner on "<<m_cluster_vector.size()<<" entries");
    m_current_cluster = 0;

    for (std::vector<ClusterPoint>::iterator it = m_cluster_vector.begin(); it != m_cluster_vector.end(); ++it){

    //for(unsigned ii = 0; ii < m_cluster_vector.size(); ++ii){
        //only execute the scan if the element has not been visited
        if (!it->getVisited()){

            it->setVisited(true);
            //DEV_INFOS("Point " + std::to_string(ii) + " visited");

            std::vector<std::vector<ClusterPoint>::iterator> local_neighbours = Step_ColourCluster::findNeighbours( *it );

            if (local_neighbours.size() < m_min_cluster_pts){
                it->setClusterID(0);
            }
            else{
                m_current_cluster++;
                Step_ColourCluster::expandCluster( *it );
            }
        }
    }
}

/**
 *
 */
std::vector<std::vector<ClusterPoint>::iterator> Step_ColourCluster::findNeighbours(ClusterPoint searchPoint){
    double colorL1 = searchPoint.getColor()[0];
    double colorA1 = searchPoint.getColor()[1];
    double colorB1 = searchPoint.getColor()[2];
    std::vector<std::vector<ClusterPoint>::iterator> local_neighbours;
    for(std::vector<ClusterPoint>::iterator it = m_cluster_vector.begin(); it != m_cluster_vector.end(); ++it){

        double colorL2 = it->getColor()[0];
        double colorA2 = it->getColor()[1];
        double colorB2 = it->getColor()[2];
        double sum = (colorL1-colorL2)*(colorL1-colorL2)*0.392157; //0.392157=100/255 because open CV stores and 8 bit int when the value is in [0,100]
        sum += (colorA1-colorA2)*(colorA1-colorA2);
        sum += (colorB1-colorB2)*(colorB1-colorB2);

        if (sum<m_clustering_distance_2){
            local_neighbours.push_back( it );
        }
    }
    return local_neighbours;
}

/**
 *
 */
void Step_ColourCluster::expandCluster(ClusterPoint expandPoint){
    expandPoint.setClusterID(m_current_cluster);
    std::vector<std::vector<ClusterPoint>::iterator>  local_neighbours = findNeighbours(expandPoint);
    while (local_neighbours.size() >= 1){
        std::vector<ClusterPoint>::iterator neighbour = local_neighbours.front();

        if (neighbour->getVisited() == false){
            neighbour->setVisited(true);
            std::vector< std::vector<ClusterPoint>::iterator > new_neighbours = findNeighbours(*neighbour);
            if (new_neighbours.size() >= m_min_cluster_pts){
                for (unsigned kk = 0; kk < new_neighbours.size(); ++kk){
                    if (!new_neighbours[kk]->getVisited()){
                        local_neighbours.push_back(new_neighbours[kk]);
                    }
                    else if (new_neighbours[kk]->getClusterID() <= 0){        // The else if block catches visited, unlabelled points
                        new_neighbours[kk]->setClusterID(m_current_cluster);  // It means we don't have to add all the points into
                    }                                                         // the neighbours list
                }
            }
        }
        if (neighbour->getClusterID()<=0){
            neighbour->setClusterID(m_current_cluster);
        }
        local_neighbours.erase(local_neighbours.begin());
    }
};
