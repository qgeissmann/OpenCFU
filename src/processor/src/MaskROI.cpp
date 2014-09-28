#include "MaskROI.hpp"

void MaskROI::update(const cv::Mat parent_image){
    if(!m_original_mat.empty() && !parent_image.empty()){
        if(( m_original_mat.rows != parent_image.rows || m_original_mat.cols != parent_image.cols)
            && (type == MASK_TYPE_FILE ||type == MASK_TYPE_DRAW))
            cv::resize(m_original_mat,m_mat,cv::Size(parent_image.cols,parent_image.rows),0,0,cv::INTER_NEAREST);
        else
            m_original_mat.copyTo(m_mat);
    }
    if(type == MASK_TYPE_AUTO && !parent_image.empty())
        this->makeAutoMask(parent_image);
}

void MaskROI::setFromPoints(const std::vector< std::pair<std::vector<cv::Point2f>,int > >& points,int width,int height){

    m_original_mat=cv::Mat(height,width,CV_8UC1,cv::Scalar(0));
    type = MASK_TYPE_DRAW;
    unsigned char j = 1;
    for(auto& i : points){
        if(i.second == MASK_TOOL_3P_CIRCLE){
            assert((i.first).size() == 3);
            std::vector<float> vec(3);
            vec = circleFrom3(i.first);
            cv::circle( m_original_mat,cv::Point(vec[0],vec[1]), vec[2], cv::Scalar(j++), CV_FILLED, 8, 0 );
        }
        else if(i.second == MASK_TOOL_CONV_POLYGON){
            assert((i.first).size() >2 );
            std::vector<std::vector<cv::Point> > pts(1);
            for(auto j : i.first){
                pts[0].push_back(cv::Point(round(j.x),round(j.y)));
            }
            cv::convexHull(pts[0],pts[0]);
            cv::drawContours(m_original_mat,pts,0,cv::Scalar(j++),-1);
        }
    }
}

void MaskROI::makeAutoMask(const cv::Mat& parent){
    cv::Mat grey;
    cv::cvtColor(parent,grey,CV_BGR2GRAY);
    cv::Mat mask(grey.size(),CV_8UC1,cv::Scalar(0));
	float r = 256.0/ (float) grey.cols ;
	cv::resize(grey,grey,cv::Size(0,0),r,r,cv::INTER_AREA);
	cv::medianBlur(grey,grey,7);
	std::vector<cv::Vec3f> out;
	cv::HoughCircles(grey, out,  CV_HOUGH_GRADIENT,2, 100, 150, 10, 75, 350);
	if(out.size()>0){
        auto& i = out[0];
        cv::circle(mask,cv::Point(i[0]/r,i[1]/r),i[2]/r,cv::Scalar(1),-1);
        m_original_mat = mask;
        m_mat = mask;
    }
    else{
        m_original_mat = cv::Mat();
        m_mat = cv::Mat();
    }
}

/** This function is to calculate the cicle matching 3 points (used to draw a circular mask) */
std::vector<float> MaskROI::circleFrom3(const std::vector<cv::Point2f>& points){
	double x1,x2,x3,y1,y2,y3,c,f,g,m,d,e,h,k,s,r;

	x1=points[0].x;
	x2=points[1].x;
	x3=points[2].x;
	y1=points[0].y;
	y2=points[1].y;
	y3=points[2].y;

	f = x3*x3-x3*x2-x1*x3+x1*x2+y3*y3-y3*y2-y1*y3+y1*y2; //formula
	g = x3*y1-x3*y2+x1*y2-x1*y3+x2*y3-x2*y1;

	if(g==0)
		m = 0;
	else
		m = (f/g);

	c = (m*y2)-x2-x1-(m*y1);	//formula
	d = (m*x1)-y1-y2-(x2*m);
	e = (x1*x2)+(y1*y2)-(m*x1*y2)+(m*x2*y1);
	h = (c/2);			//formula
	k = (d/2);
	s = (((h)*(h))+((k)*(k))-e);
	r = pow(s,0.5);


	std::vector<float>vec(3);
	vec[0] = pow(pow(h,2),.5);
	vec[1] = pow(pow(k,2),.5);
	vec[2] = r;
	return vec;
}
