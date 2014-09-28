#include "Features.hpp"

Features::Features():m_nfeatures(6+7){}

void Features::calcFeatures(const ContourFamily& fam,cv::Mat out){
    std::vector<cv::Point> contour ;
    this->smoothContour(fam.contours[0],contour);
    unsigned int s = fam.contours.size();
    unsigned int np = contour.size();
    float perim;
    float area;
    assert (np>6);

    perim = cv::arcLength( contour,true);
    area = cv::contourArea(contour);

    cv::Point2f WH = Features::calculateWH(contour);
    cv::Point2f PA_hull = calcHullPerimArea(contour);

    float perim_hole = 0;
    float area_hole = 0;
    for(unsigned int i=1;i != s; ++i){
        if(fam.contours[i].size() > 6){
            perim_hole += cv::arcLength(fam.contours[i],true);
            area_hole +=  cv::contourArea(fam.contours[i]);
        }
    }
    if(area_hole  >= area)
        area = 1;
    else
        area -= area_hole;
    perim += perim_hole;
    PA_hull.y -= area_hole;
    PA_hull.x += perim_hole;
    float matData[m_nfeatures];
    int p=0;

    matData[p++] = perim * perim / area;
    matData[p++] = (PA_hull.y - area) / PA_hull.y;
    matData[p++] = (PA_hull.x - perim) / PA_hull.x;
    matData[p++] = area_hole / (area+area_hole);
    matData[p++] = perim_hole / (perim+perim_hole);
    matData[p++] = WH.x / (WH.y + WH.x);

    double hu_moms[7];
    cv::HuMoments(cv::moments(contour),hu_moms);
    for(int i = 0; i<7;i++){
        matData[i + p] = hu_moms[i];

    }
    cv::Mat layers(1,m_nfeatures,CV_32F,(void*)matData);
    layers.copyTo(out);

}


cv::Point2f Features::calcHullPerimArea(const std::vector<cv::Point>& contour){
    std::vector<cv::Point> hull;
    cv::convexHull(contour, hull);
    return cv::Point(cv::arcLength( contour,true),cv::contourArea(hull));
}

cv::Point2f Features::calculateWH(const std::vector<cv::Point>& contour){
    cv::Point2f rRect[4];
    cv::minAreaRect(contour).points(rRect);

    float A,B;
    A = calcTwoPointDist(rRect[0],rRect[1]) + 1;
    B = calcTwoPointDist(rRect[1],rRect[2]) + 1;

    return cv::Point(std::max(A,B),std::min(A,B));
}


inline float Features::calcTwoPointDist(const cv::Point P0,const cv::Point P1){
        float Ax,Ay;
        Ax = P0.x - P1.x;
        Ax *= Ax;
        Ay = P0.y - P1.y;
        Ay *= Ay;
        return sqrt(Ax + Ay) / 2;
}


void Features::smoothContour(const std::vector<cv::Point>& in ,std::vector<cv::Point>& out){

    /* We define kenrel size as {number of points}/20*/
    int k = 2*(in.size()/40) + 1;
    /* however, k will not be higher than 99. Smoothing is important mainly for small shapes.
       Also, it is very slow as k gets large */
    if(k>99)
        k=99;
    /*k cannot be lower than 3*/
    if(k>2){
        /* Temporary matrix*/
        cv::Mat in_mat,smooth_mat;
        /* We copy the data of contour into the matrix (no hard copy here, so no need to copy back the data to the vector)*/
        in_mat = cv::Mat(in,false);

        /* These line is to solve the border effect (a contour is circular)*/
        cv::copyMakeBorder(in_mat,smooth_mat, (k-1)/2,(k-1)/2 ,0, 0, cv::BORDER_WRAP);

        /* Linear kernel application*/
        cv::blur(smooth_mat, smooth_mat, cv::Size(1,k),cv::Point(-1,-1));

        /* We remove the artificial border put two lines ago  */
        smooth_mat.rowRange(cv::Range((k-1)/2,1+smooth_mat.rows-(k-1)/2)).copyTo(out);
    }
    else
        out = in;
}
