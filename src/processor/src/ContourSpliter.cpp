#include "ContourSpliter.hpp"

void ContourSpliter::split(std::vector<ContourFamily>& contour_fams,std::vector<signed char>& categ){//,
    unsigned int hi = contour_fams.size();
    std::vector<std::vector<ContourFamily> > list_contour_fams(hi);

    #pragma omp parallel for schedule(dynamic)
    for(unsigned int i=0; i < hi; ++i ){
        signed char cat_it = categ[i];
        std::vector<ContourFamily> tmp_v = list_contour_fams[i];
        if(cat_it == 'M'){
            splitOneCont(contour_fams[i], tmp_v);
        }
        else if(cat_it == 'S' ){
            tmp_v.resize(1);
            tmp_v[0] = contour_fams[i];
        }
        list_contour_fams[i] = tmp_v;
    }



    unsigned int siz = 0;
    for(unsigned int i=0; i < hi; ++i )
        siz += list_contour_fams[i].size();

    std::vector<ContourFamily> tmp_contour_fams(siz);
    std::vector<signed char> tmp_categ(siz);

    unsigned int k = 0;


    for(unsigned int i=0; i < hi; ++i ){
        unsigned int ncl = list_contour_fams[i].size();

        for(unsigned int j=0; j < ncl; ++j ){
            tmp_contour_fams[k] = list_contour_fams[i][j];
            tmp_categ[k] = categ[i];
            tmp_contour_fams[k].n_per_clust = ncl;
            k++;

        }
    }

    std::swap(tmp_contour_fams,contour_fams);
    std::swap(tmp_categ,categ);
}

void ContourSpliter::makeWatershedLabel(const cv::Mat& binary, const cont_chunk& peaks_conts, cv::Mat& labels){
    cv::threshold(binary,labels,0,1,cv::THRESH_BINARY);
    for(unsigned int k=0;k<peaks_conts.size();k++)
        cv::drawContours(labels,peaks_conts,k,cv::Scalar(k+2),-1,8);
}

void ContourSpliter::findPeaks(const cv::Mat& binary, cv::Mat& distance_map, cont_chunk& peaks_conts){
    cv::Mat tmp_mat,peaks;
    cv::distanceTransform(binary,distance_map,CV_DIST_L2,CV_DIST_MASK_5);
    cv::dilate(distance_map,peaks,cv::Mat(),cv::Point(-1,-1),3);
    cv::dilate(binary,tmp_mat,cv::Mat(),cv::Point(-1,-1),3);
    peaks = peaks - distance_map;
    cv::threshold(peaks,peaks,0,255,cv::THRESH_BINARY);
    peaks.convertTo(peaks,CV_8U);
    cv::bitwise_xor(peaks,tmp_mat,peaks);
    cv::dilate(peaks,peaks,cv::Mat(),cv::Point(-1,-1),1);
    cv::findContours(peaks, peaks_conts, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    distance_map.convertTo(distance_map,CV_8U);
}

void ContourSpliter::splitOneCont(const ContourFamily& in, std::vector<ContourFamily>& out){

    cv::Rect rect = cv::boundingRect(in.contours[0]);
    cv::Mat miniTmp(rect.height,rect.width,CV_8U,cv::Scalar(0));
    cv::drawContours(miniTmp,in.contours,-1,cv::Scalar(255),-1,8,in.hierarchies, INT_MAX, cv::Point(-rect.x,-rect.y));
    cv::copyMakeBorder(miniTmp,miniTmp, 4,4,4,4, cv::BORDER_CONSTANT, cv::Scalar(0));

    cv::Mat distance_map;
    cont_chunk peaks;
    findPeaks(miniTmp,distance_map,peaks);

    unsigned int n_peaks = peaks.size();
    cv::Mat label_mat;
    makeWatershedLabel(miniTmp,peaks,label_mat);
    watershedLike(label_mat,distance_map,n_peaks,1.6);
    std::vector<ContourFamily> tmp_out;
    tmp_out.reserve(n_peaks);
    for(unsigned int k=0;k != n_peaks; ++k){
        cv::Mat tmp;
        cont_chunk tmpc;
        cv::inRange(label_mat, k+2,k+2, tmp);
        cv::findContours(tmp, tmpc, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE,cv::Point(rect.x-4,rect.y-4));
        tmp_out.push_back(ContourFamily(tmpc));
    }
    std::swap(out,tmp_out);
}



void ContourSpliter::watershedLike(cv::Mat &mask,cv::Mat& gray,int nlabs,double maxAreaModif){
    /* Mask is 0 where no objects exist, 1 in undefined womes and >1 for attributed zones/labels*/
    /* toUse is >0 for mpixel that have not yet been used as seed, and ==0 for the rest*/
    cv::Mat toUse, tmp;
    std::vector<int> areaCount(nlabs),peakValSQ(nlabs), maxArea(nlabs);
    std::vector<cv::Point> center(nlabs);
    for(unsigned int j=0; j<areaCount.size();j++){
        areaCount[j] = 0;
        peakValSQ[j] = 0;
    }

    mask.copyTo(toUse);
    unsigned int nc=mask.cols;
    unsigned int nl=mask.rows;

    /*define the peaks heigh in gray*/
    for(unsigned int j=0; j<nl;j++){
        for(unsigned int i=0; i<nc;i++){
            char newVal =*(mask.data+j*mask.step+i*mask.elemSize());
            if(newVal > 1){
                if(peakValSQ[newVal-2] < *(gray.data+(j)*gray.step+(i)*gray.elemSize()) ){
                   peakValSQ[newVal-2] = *(gray.data+(j)*gray.step+(i)*gray.elemSize());
                   peakValSQ[newVal-2] = peakValSQ[newVal-2] * peakValSQ[newVal-2];
                   center[newVal-2]=cv::Point(j,i);

                }
            }
        }
    }
    for(unsigned int j=0; j<areaCount.size();j++){
        maxArea[j] = maxAreaModif * peakValSQ[j]*3.1416;
    }
    bool on =true;
    int iter = 0;
    while(on){
        mask.copyTo(tmp);
        on =false;
        for(unsigned int j=0; j != nl; ++j){
            for(unsigned int i=0; i != nc; ++i){
                /* find pixels that are labels (mask) and unused(toUse)*/
                if(*(mask.data+j*mask.step+i*mask.elemSize()) > 1 && *(toUse.data+j*toUse.step+i*toUse.elemSize()) > 0){
                    /*for each neighbourgs*/
                    for(int m=-1; m != 2;++m){
                        for(int n=-1; n !=2;n++){
                            bool test = !(n == 0 && m==0 ) || (n==0 || m==0);
                                if( test && *(tmp.data+(j+m)*tmp.step+(i+n)*tmp.elemSize()) == 1){
                                switch (*(mask.data+(j+m)*mask.step+(i+n)*mask.elemSize())){
                                    case 0:
                                        break;
                                    /* if the mask in markable*/
                                    case 1:
                                        /* if the neighbour value in gray is lower or equal to the target*/
                                        if(*(gray.data+(j+m)*gray.step+(i+n)*gray.elemSize()) <= *(gray.data+j*gray.step+i*gray.elemSize()) ){
                                            char newVal =*(mask.data+j*mask.step+i*mask.elemSize());
                                            int xd = (j+m)-center[newVal-2].x;
                                            int yd = (i+m)-center[newVal-2].y;
                                            if(areaCount[newVal-2] < maxArea[newVal-2] && xd*xd+yd*yd < maxAreaModif*peakValSQ[newVal-2]){
                                                *(tmp.data+(j+m)*tmp.step+(i+n)*tmp.elemSize()) = newVal;
                                                ++areaCount[newVal-2];
                                            }
                                        }
                                        break;
                                    default:
                                        break;
                                }
                            }
                        }
                    }
                    *(toUse.data+j*toUse.step+i*toUse.elemSize()) = 0;
                    on = true;
                }
            }
        }
        tmp.copyTo(mask);
        ++iter;
    }
}


