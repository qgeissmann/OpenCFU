#ifndef DATAMAKER_H
#define DATAMAKER_H

#include "defines.hpp"
#include "dirent.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "ContourFamily.hpp"
#include "Features.hpp"

class DataMaker
{
    public:
        DataMaker();
        DataMaker(const std::string& lib,const std::string& lib_ps);
        void makeData(cv::Mat& out, std::vector<signed char>& category );
        void makeDataPS(cv::Mat& out, std::vector<signed char>& category );
    protected:
        void makeFileList(std::vector<std::string>& out,const char direct[]);
        signed char findCategFromName(const std::string& str);
        void makeFeaturesMatrix(const std::vector<ContourFamily>& contour_fams,cv::Mat& dst);
        void makeContourChunksVect(const cv::Mat& src,std::vector<ContourFamily>& contour_fams);
        void preFilterContourSize(std::vector<ContourFamily>& contour_fams);
    private:
        Features m_featureMaker;
        std::string m_library;
        std::string m_library_ps;
        bool m_rotate_scale;
};
#endif // DATAMAKER_H
