#include "Result.hpp"
#include <sstream>

OneObjectRow::OneObjectRow():
m_n_in_clust(0),m_valid(false),m_ROI(1), cluster_class(1),m_color_cluster_ID(0){}

OneObjectRow::OneObjectRow(ContourFamily cont_fam,const cv::Mat& raw_img):
    m_n_in_clust(cont_fam.n_per_clust),m_valid(true),m_ROI(1),cluster_class(1),m_color_cluster_ID(0)
    {

        m_rrect = cv::minAreaRect(cont_fam.contours[0]);
        m_rad = Features::calculateWH(cont_fam.contours[0]).x;
        cv::Rect rect = cv::boundingRect(cont_fam.contours[0]);

        cv::Mat mini_mask(rect.height,rect.width,CV_8UC1,cv::Scalar(0));
        cv::Mat mini_img;
        raw_img(rect).copyTo(mini_img);
        cv::drawContours(mini_mask,cont_fam.contours,0,cv::Scalar(255),-1,8, cont_fam.hierarchies, 2, cv::Point(-rect.x,-rect.y));

        cv::Mat mean, stddev;
        cv::meanStdDev(mini_img, mean, stddev, mini_mask);
        m_area = cv::countNonZero(mini_mask);
        stddev +=1;

        m_BGR_mean = mean.at<cv::Scalar>(0,0);  //m_BGR_mean[0] =  blue channel, [1] = green channel, [2] = Red channel, (0-255)
        m_BGR_sd = stddev.at<cv::Scalar>(0,0);



        cv::Mat one_pix(1,1,CV_8UC3,m_BGR_mean);
        cv::Mat one_pix_hls, one_pix_lab;           //modified NJL 11/AUG/2014

        cv::cvtColor(one_pix,one_pix_hls,cv::COLOR_BGR2HLS);
        cv::Scalar mean_sc = cv::mean(one_pix_hls); //?

        m_hue_mean =((int) mean_sc [0])*2;
        m_sat_mean = (int) mean_sc [2];

        //added NJL 11/AUG/2014
        cv::cvtColor(one_pix, one_pix_lab, cv::COLOR_BGR2Lab);
        m_LAB_mean = cv::mean(one_pix_lab);



    }

std::string OneObjectRow::print() const{
    std::stringstream ss;
    cv::Point2f center = (this->getPoint(0) + this->getPoint(2) ) * 0.5;
    ss   << this->isValid()<<","
                    << center.x<<","
                    <<center.y<<","
                    << this->getROI()<<","
                    << this->getColorClusterID()<<","
                    << this->getNInClust()<<","
                    << this->getArea()<<","
                    << this->getRadius()<<","
                    << this->getHue()<<","
                    << this->getSat()<<","
                    << this->getBGRMean()[2]<<","
                    << this->getBGRMean()[1]<<","
                    << this->getBGRMean()[0]<<","
                    << this->getBGRSd()[2]<<","
                    << this->getBGRSd()[1]<<","
                    << this->getBGRSd()[0];

    return  ss.str();
}

std::string OneObjectRow::printHeader(){
    std::stringstream ss;
    ss << "IsValid"<<","
                << "X"<<","
                << "Y"<<","
                << "ROI"<<","
                << "Colour_group"<<","
                << "N_in_clust"<<","
                << "Area"<<","
                << "Radius"<<","
                << "Hue" <<","
                << "Saturation" <<","
                << "Rmean" <<","
                << "Gmean" <<","
                << "Bmean" <<","
                << "Rsd" <<","
                << "Gsd" <<","
                << "Bsd";

    return ss.str();
}

void Result::reset(size_t s){
    v.clear();
    v.resize(s);
    m_n_valid = 0;
}

void Result::add_at(const OneObjectRow& row,const unsigned int i){
    v[i] = row;
    if(row.isValid())
        m_n_valid++;
}


 void Result::applyFilter(const std::vector<bool>& valid){
    if (valid.empty())
        return;

    assert(valid.size() == v.size());

    m_n_valid = 0;
    for(unsigned int i = 0; i != valid.size(); ++i){
        int test = (int)v[i].getGUIValid();
        if(test > 0){
            v[i].setValid(true);
            ++m_n_valid;
        }

        else if(test == -1){
            if(!valid[i])
               v[i].setValid(false);
            else{
               ++m_n_valid;
               v[i].setValid(true);
            }
        }

        else if(test == 0)
            v[i].setValid(false);
    }
}

void Result::applyFilter(const std::vector<int>& valid){

    if (valid.empty())
        return;

    assert(valid.size() == v.size());

    m_n_valid = 0;
    for(unsigned int i = 0; i != valid.size(); ++i){
        int test = (int)v[i].getGUIValid();
        if(test > 0){
            v[i].setValid(true);
            ++m_n_valid;
           ////MODIF FOR ROI ATTRIBUTION
           v[i].setROI(valid[i]);
        }

        else if(test == -1){
            if(!valid[i]){
               v[i].setValid(false);
               v[i].setROI(0);
            }
            else{
               ++m_n_valid;
               v[i].setValid(true);
               ////MODIF FOR ROI ATTRIBUTION
               v[i].setROI(valid[i]);
            }
        }

        else if(test == 0)
            v[i].setValid(false);
    }
}

//NJL 13/AUG/2014
/**
 *
 * \param vector<pair<int,int>> values of row id and cluster to update each oor with
 */
void Result::recluster(std::vector< std::pair<int,int> > clustered){
    if (clustered.empty()){
        return;
    }

    std::sort(clustered.begin(), clustered.end());
    std::vector<bool> valid;
    for(std::vector< std::pair<int,int> >::const_iterator it = clustered.begin(); it != clustered.end(); ++it){
        v[it->first].setColorClusterID(it->second);
        if (it->second == 0) //if not in a cluster, point is invalid
            valid.push_back(false);
        else
            valid.push_back(true);
    }
    applyFilter(valid);
    ColorProcessing(false);
}

/**
 *  ClusterOrder This function analyses the colours of clusters, with two ends
 *  It assigns a mean colour to the cluster, and it allows the clusters to be sorted
 *  by luminosity. Note that this overwrites the default preference to be sorted by
 *  quantity
 *
 *  @param bool ordering Enables the ordering of colonies by luminosity
 */

void Result::ColorProcessing(bool ordering){
//create structure cluster 1: [colour, colour, colour...]
//                 cluster 2: [colour, ....]
    std::unordered_map< int, std::vector<cv::Scalar> > clusterColors;

    for (std::vector<OneObjectRow>::iterator it = v.begin(); it != v.end(); ++it){
        int ID = it->getColorClusterID();
        if (ID != 0){
            std::unordered_map< int, std::vector<cv::Scalar> >::iterator loc = clusterColors.find(it->getColorClusterID());
            if ( loc == clusterColors.end() ) {
                std::vector<cv::Scalar> cc;
                cc.push_back( it->getLABMean() );
                clusterColors.emplace( ID, cc );
            }
            else {
                (loc->second).push_back( it-> getLABMean() );
            }

        }
    }


//find average colour for each cluster
    std::vector< std::pair< int, std::vector<double> > > clusterMeanColors;
    for ( std::unordered_map< int, std::vector<cv::Scalar> >::iterator it = clusterColors.begin(); it != clusterColors.end(); ++it){

        std::vector<double> target(3, 0.);
        double length = (double) (it->second).size();
        std::vector< cv::Scalar > sources = it->second;

        for (cv::Scalar source : sources){
            target[0] += source[0];
            target[1] += source[1];
            target[2] += source[2];
        }
        target[0] /= length;
        target[1] /= length;
        target[2] /= length;

        clusterMeanColors.push_back( std::make_pair( it->first, target ) );

    }

//create translation map
//cluster 1 has lowest L* and so on.
    //Sorting, need to define a custom comparator


    std::sort(clusterMeanColors.begin(), clusterMeanColors.end(), pairCompare);

    std::unordered_map<int, std::pair<int,cv::Scalar> > translationTable;
    cv::Scalar emptyPixel(0,0,0);
    translationTable.emplace(0, std::make_pair(0, emptyPixel));
    int currentClusterNumber = 1;


    for (std::vector< std::pair< int, std::vector<double> > >::iterator it = clusterMeanColors.begin(); it != clusterMeanColors.end(); ++it){
        cv::Scalar pixelColour((int) it->second[0], (int) it->second[1], (int) it->second[2]);
        cv::Mat onepixBGR;
        cv::Mat onepixLAB(1,1,CV_8UC3,pixelColour);
        cv::cvtColor(onepixLAB, onepixBGR, cv::COLOR_Lab2BGR);
        cv::Scalar meanBGR = cv::mean(onepixBGR);

        translationTable.emplace(it->first, std::make_pair(currentClusterNumber++, meanBGR));
    }

//Apply translation map to each row
//add RGB cluster colour to each row.
    m_roi_data.clear();
    for (std::vector<OneObjectRow>::iterator it = v.begin(); it != v.end(); ++it){
        std::unordered_map<int, std::pair<int,cv::Scalar> >::iterator loc = translationTable.find(it->getColorClusterID());

        if (ordering) {it->setColorClusterID( (loc->second).first );} //Ordering by luminosity if requested
        it->setClusterColor( (loc->second).second );
        int roi = it->getROI();
        m_roi_data.addROIClusterData(0).addCluster(it->getColorClusterID(), it->getClusterColor());
        if (roi != 0){
            m_roi_data.addROIClusterData(roi).addCluster(it->getColorClusterID(), it->getClusterColor());
        }
    }
}

bool pairCompare(const std::pair< int, std::vector<double> >& a, const std::pair< int, std::vector<double> >& b){return ( a.second.at(0) < b.second.at(0) );}


//NJL 01/SEP/2014
/**
 *
 * Set all members to cluster 0
 */
void Result::uncluster(){
    for(std::vector<OneObjectRow>::iterator it = v.begin(); it != v.end(); ++it){
        it->setColorClusterID(0);
    }
}

void Result::applyGuiFilter(const cv::Mat& valid){
    if (valid.empty()){
        DEV_INFOS("Empty filter!");
        return;
    }

    std::vector<bool> boo(valid.rows);

    m_n_valid = 0;
    unsigned int nrow = (unsigned)valid.rows;
    for(unsigned int i = 0; i < nrow; i++){
        signed char val = valid.at<signed char>(i,0);
        v[i].setGUIValid(val);
        boo[i] = true;
    }

    applyFilter(boo);

}

ClusterData::ClusterData(){
    this->addCluster(0, cv::Scalar(0,0,0)); //initiate with empty cluster 0
    }

const std::string ClusterData::str() const{
    std::stringstream ss;
    //this makes a python-dict like output for the saved string.
    std::vector<int> keys;
    for (auto &it : m_clusters){
        keys.push_back(it.first);
    }
    int maxClustIndex = keys.begin() == keys.end() ? 0 : *std::max_element(keys.begin(),keys.end());

    ss<<"{ ";
    if ( maxClustIndex >= 1){
        for (int ii = 1; ii != maxClustIndex+1; ++ii ){
            ss  <<(ii)<<": { "
                <<"'num' : "<<clusterPop(ii)<<", "
                <<"'r' : "<<(int) clusterColor(ii)[2]<<", "
                <<"'g' : "<<(int) clusterColor(ii)[1]<<", "
                <<"'b' : "<<(int) clusterColor(ii)[0]<<" }";
            if (ii != maxClustIndex)
                ss<<",  ";
        }
    }
    ss<<" }";
    return ss.str();
}


