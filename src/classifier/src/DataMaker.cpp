#include "DataMaker.hpp"
#include <algorithm>

DataMaker::DataMaker() : m_library(TRAINING_SET_IMG),m_library_ps(TRAINING_SET_IMG_PS){}

DataMaker::DataMaker(const std::string& lib,const std::string& lib_ps):
m_library(lib.c_str()),
m_library_ps(lib_ps.c_str()),
m_rotate_scale(true)
{}

void DataMaker::makeFileList(std::vector<std::string>& out,const char direct[]){
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (direct)) != NULL) {
      /* print all the files and directories within directory */
      while ((ent = readdir(dir)) != NULL) {
        std::stringstream ss;
        ss<<direct;
        ss<<ent->d_name;
        std::string str = ss.str();
        int found = str.rfind(".png");
        if(found != std::string::npos)
            out.push_back(str);
      }
      std::sort(out.begin(), out.end());
      closedir (dir);
    }
    else {
      return;
    }
}

signed char DataMaker::findCategFromName(const std::string& str){
    int neg = str.rfind("Neg_");
    int sing = str.rfind("Sing_");
    int mult = str.rfind("Mult_");
    signed char toRet = -1;
    if(neg != std::string::npos)
        toRet = 'N';
    else if(sing != std::string::npos)
        toRet = 'S';
    else if(mult != std::string::npos)
        toRet = 'M';
    return toRet;
}

void DataMaker::makeDataPS(cv::Mat& out, std::vector<signed char>& category ){
    out =cv::Mat();
    category.resize(0);
    std::vector<std::string> allFiles;
    makeFileList(allFiles,m_library_ps.c_str());
    std::vector< std::vector<ContourFamily> >contour_fams;
    for(auto& str : allFiles){
        cv::Mat src = cv::imread(str,0);
        assert(!src.empty());
        signed char categ = findCategFromName(str);
        cv::Mat img0, img;
        src.copyTo(img0);
        for(int j=0; j<4;j++){
                if (j==0)
                    cv::flip(img0,img,0);
                else if(j==1)
                    cv::flip(img0,img,1);
                else if(j==2)
                    cv::flip(img0,img,2);
                else
                    img0.copyTo(img);

            for(int i=0; i != 4;++i){
                cv::transpose(img,img);
                cv::flip(img,img,0);
                std::vector<ContourFamily>  tmp_fam;
                makeContourChunksVect(img,tmp_fam);
                preFilterContourSize(tmp_fam);
                cv::Mat tmp_mat;
                makeFeaturesMatrix(tmp_fam,tmp_mat);
                out.push_back(tmp_mat);
                for(unsigned int k=0; k != tmp_fam.size();++k){
                    category.push_back(categ);
                }
            }
        }
    }
}


void DataMaker::makeData(cv::Mat& out, std::vector<signed char>& category ){
    out =cv::Mat();
    category.resize(0);
    std::vector<std::string> allFiles;
    makeFileList(allFiles,m_library.c_str());
    std::vector< std::vector<ContourFamily> >contour_fams;
    for(auto& str : allFiles){
        DEV_INFOS(str);
        cv::Mat src = cv::imread(str,0);
        assert(!src.empty());
        signed char categ = findCategFromName(str);
        cv::Mat img0, img;
        src.copyTo(img0);
        for(int j=0; j<4;j++){
                if (j==0)
                    cv::flip(img0,img,0);
                else if(j==1)
                    cv::flip(img0,img,1);
                else if(j==2)
                    cv::flip(img0,img,2);
                else
                    img0.copyTo(img);

            for(int i=0; i != 4;++i){
                cv::transpose(img,img);
                cv::flip(img,img,0);
                std::vector<ContourFamily>  tmp_fam;
                makeContourChunksVect(img,tmp_fam);
                preFilterContourSize(tmp_fam);
                cv::Mat tmp_mat;
                makeFeaturesMatrix(tmp_fam,tmp_mat);
                out.push_back(tmp_mat);
                for(unsigned int k=0; k != tmp_fam.size();++k){
                    category.push_back(categ);
                }
            }
        }
    }
}

void DataMaker::makeFeaturesMatrix(const std::vector<ContourFamily>& contour_fams,cv::Mat& dst){
    unsigned int n = contour_fams.size();
    unsigned int n_features = m_featureMaker.getNFeature();
    dst = cv::Mat(n,n_features,CV_32F);
    for(unsigned int i = 0; i < n;i++){
        cv::Mat tmp_row(1,n_features,CV_32F);
        m_featureMaker.calcFeatures(contour_fams[i],tmp_row);
        tmp_row.copyTo(dst.row(i));
    }
}

void DataMaker::makeContourChunksVect(const cv::Mat& src,std::vector<ContourFamily>& contour_fams ){
    cont_chunk contours_chunk;
    hier_chunk hieras_chunk;
    cv::Mat thrd;
    cv::threshold(src,thrd,128,255,cv::THRESH_BINARY);
    cv::findContours(thrd, contours_chunk, hieras_chunk, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);
    unsigned int c=0;
    unsigned int CC = contours_chunk.size();
    while ( c < CC){
        /*if this is not the lastest non-hole*/
        unsigned int nHoles = 0;
        if( hieras_chunk[c][0] > 0)
            nHoles = hieras_chunk[c][0]-c-1;
        else
            nHoles = CC - (c+1);
        contour_fams.push_back(ContourFamily(cont_chunk(contours_chunk.begin()+c,contours_chunk.begin()+c+nHoles+1)));
        c += nHoles+1;
    }
}

void DataMaker::preFilterContourSize(std::vector<ContourFamily>& contour_fams){
    std::vector<ContourFamily> tmp_fams;
    tmp_fams.reserve(contour_fams.size());
    for(ContourFamily& i : contour_fams){
        if(i.contours[0].size() <= 6)
            continue;
        else
            tmp_fams.push_back(i);
    }
    std::swap(tmp_fams,contour_fams);
}
