#ifndef CONTOURFAMILY_H
#define CONTOURFAMILY_H

#include "defines.hpp"
#include "opencv2/core/core.hpp"


typedef std::vector<std::vector<cv::Point> > cont_chunk;
typedef std::vector<cv::Vec4i> hier_chunk;

class ContourFamily{
    public:
        ContourFamily();
        ContourFamily(const cont_chunk& cc);
        cont_chunk contours;
        hier_chunk hierarchies;
        char predicted_categ;
        int n_per_clust;
    private:
        void makeHierarchies(unsigned int nHoles);
};
#endif
