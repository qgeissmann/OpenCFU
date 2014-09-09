#include "ContourFamily.hpp"

ContourFamily::ContourFamily():
n_per_clust(0)
{}

ContourFamily::ContourFamily(const cont_chunk& cc):
contours(cc),
n_per_clust(0)
{
    this->makeHierarchies(cc.size() -1 );
}

void ContourFamily::makeHierarchies(unsigned int nHoles){
    if (nHoles ==0){
        hierarchies.push_back(cv::Vec4i(-1,-1,-1,-1));
    }
    else{
        hierarchies.reserve(nHoles+1);
        hierarchies.push_back(cv::Vec4i(-1,-1,1,-1));

        int zeroth,first;

        for(unsigned int h = 1; h != nHoles+1; h++){
            if(h == nHoles)
                zeroth = -1;
            else
                zeroth = h+1;
            if(h == 1)
                first = -1;
            else
                first = h-1;
            hierarchies.push_back(cv::Vec4i(zeroth,first,-1,0));
        }
    }
}
