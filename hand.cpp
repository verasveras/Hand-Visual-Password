#include <opencv\cv.h>
#include <opencv\highgui.h>
#include "hand.h"

using namespace cv;
using namespace std;


//////////////////////////////////////////////////////////////////////////
// Constructors
Hand::Hand(){
	com = cvPoint(0,0);
	size = 0;
	locationI = 1;
}
Hand::Hand(Mat image1){
	image = image1;
	image_binary = makeBinary(image1);
	com = cvPoint(0,0);
	size = 0;
    locationI = 1;
	width = image.cols;
	height = image.rows;
}



//////////////////////////////////////////////////////////////////////////
// Calculations

int Hand::centerOfMass () {
	
	Mat im_binary = image_binary;
    int Xsum = 0;
	int Ysum = 0;
	int width = im_binary.cols;
	int height = im_binary.rows;
	double total = 0;
	
	Scalar pixel;
	for (int i = 1; i < width; i++){
		for (int j = 1; j < height; j++){
			pixel = im_binary.at<uchar>(j, i);
			if (pixel.val[0] > 0) {
				Xsum += i;
				Ysum += j;
				total += 1;
			}
		}
	}

	double Xcoord = Xsum/total; 
	double Ycoord = Ysum/total;

	com = cvPoint(Xcoord, Ycoord);
	
	return 0;
}
Mat Hand::makeBinary(Mat image){

	Mat binary;
	Mat image_gray;
	cvtColor(image,image_gray,CV_RGB2GRAY);
	binary = image_gray > 130;

	return binary;
}
int Hand::assignLocation () {
	
    /// Divide the image into five boxes. ///////////////////////////////
	/// Middle box, upper left/right, lower left/right boxes.
	int height_mid = height/2;
	int width_mid = width/2;
	int height_sixth = height/6;
	int width_sixth = width/6;

	// CvPoint centerofMass1 = com;
	CvPoint* centerofMass = &com;
	int Xcoord = centerofMass->x;
	int Ycoord = centerofMass->y;

    // middle. middle point at (width_mid, height_mid).
	if (Xcoord >= width_mid - width_sixth && Xcoord <= width_mid + width_sixth && 
		Ycoord >= height_mid - height_sixth &&Ycoord <= height_mid + height_sixth){
			locationS = "Middle";
			locationI = 1;
	}
	// upperright
	else if (Xcoord > width_mid && Ycoord < height_mid){
		locationS = "Upperright";
		locationI = 2;
	}

	// upperleft
	else if (Xcoord < width_mid && Ycoord < height_mid){
		locationS = "Upperleft";
		locationI = 3;
	}
	//lowerright
	else if (Xcoord > width_mid && Ycoord > height_mid){
		locationS = "Lowerright";
		locationI = 5;
	}
	//lowerleft
	else if (Xcoord < width_mid && Ycoord > height_mid){
		locationS = "Lowerleft";
		locationI = 4;
	}

	// cout << locationS << endl;
	// cout << locationI << endl;

	return 0;
}
int Hand::findHandContours(){

	Mat src; Mat src_gray;
	src = image;
    cvtColor( src, src_gray, CV_BGR2GRAY );
	src_gray = src_gray > 150;
	bitwise_not ( src_gray, src_gray );
    blur( src_gray, src_gray, Size(3,3) );
	
    Mat src_copy = src.clone();
    // Mat threshold_output;
    // vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    // Find contours
    threshold( src_gray, threshold_output, 200, 255, THRESH_BINARY );
    findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

	return 0;
}
int Hand::findBiggestContour(){

	largest_area=0;
	largest_index=0;
	double area_test;

	for (int i = 0; i < contours.size(); i++ ) {
       double area_test = contourArea(contours[i], false); 

       if (area_test > largest_area && area_test < (int) 400000){
	       largest_area = area_test;
           largest_index = i;
       }
   
    }

	return largest_index;
}
int Hand::findConvexHull(){
	return 0;
}
int Hand::findContourTop(){

	top = height;
	vector<Point> cont = contours[largest_index];
	Point point;

	for (int i = 0; i < cont.size(); i++){
		point = cont[i];
		if (point.y < top){
			top = point.y;
		}
	}

	CvPoint *start = new CvPoint;
	start -> x = 0;
	start -> y = top;

	CvPoint *end = new CvPoint;
	end -> x = width;
	end -> y = top;
	
	line(image_binary, *start, *end, 255, 1, 8, 0); 
	return top;

}
int Hand::findContourBottom(){

	bottom = 0;
	vector<Point> cont = contours[largest_index];
	Point point;

	for (int i = 0; i < cont.size(); i++){
		point = cont[i];
		if (point.y > bottom){
			bottom = point.y;
		}
	}

	CvPoint *start = new CvPoint;
	start -> x = 0;
	start -> y = bottom;

	CvPoint *end = new CvPoint;
	end -> x = width;
	end -> y = bottom;
	
	line(image_binary, *start, *end, 255, 1, 8, 0); 
	return bottom;
}
int Hand::fingerBottomCount(){

	int bottom_fingers = 0;
	int third_bottom = bottom - (int) ((bottom-top)/3);

    int y = third_bottom;
	int x;
	int remember = 0;

	for (x = 0; x < width; x++){
		
		if(remember != (int)image_binary.at<uchar>(y,x)) bottom_fingers++;
			// cout << (int)image_binary.at<uchar>(y,x) << endl;

		if ((int)image_binary.at<uchar>(y,x) == 255) remember = 255;
		else remember = 0;
	}

	CvPoint *start = new CvPoint;
	start -> x = 0;
	start -> y = third_bottom;

	CvPoint *end = new CvPoint;
	end -> x = width;
	end -> y = third_bottom;
	
	line(image_binary, *start, *end, 255, 1, 8, 0);

	bottom_fingers = bottom_fingers/2;

	return bottom_fingers;

}
int Hand::fingerTopCount(){

	int top_fingers = 0;
	int third_top = top + (int) ((bottom-top)/3);

    int y = third_top;
	int x;
	int remember = 0;

	for (x = 0; x < width; x++){
		// if ((int)image_binary.at<uchar>(y,x) > remember || (int)image_binary.at<uchar>(y,x) < remember ){
		
		if(remember != (int)image_binary.at<uchar>(y,x)) top_fingers++;
			// cout << (int)image_binary.at<uchar>(y,x) << endl;

		if ((int)image_binary.at<uchar>(y,x) == 255) remember = 255;
		else remember = 0;
	}

	CvPoint *start = new CvPoint;
	start -> x = 0;
	start -> y = third_top;

	CvPoint *end = new CvPoint;
	end -> x = width;
	end -> y = third_top;
	
	line(image_binary, *start, *end, 255, 1, 8, 0); 
	

	return top_fingers/2;

}


////////////////////////////////////////////////////////////////////////
// Information Retrival

int Hand::getLocationI(){
	return locationI;
}
int Hand::getContourArea(){
	return largest_area;
}
String Hand::getLocationS(){
	return locationS;
}
CvPoint Hand::getCenterOfMass(){
	return com;
}
int Hand::getHeight(){
	return height;
}
int Hand::getWidth(){
	return width;
	}
vector<vector<Point> > Hand::getCountors(){
	return contours;
}
vector<vector<Point> > Hand::getHullP(){
	return HullP;
}
vector<vector<int> > Hand::getHullI(){
	return HullI;
}



//////////////////////////////////////////////////////////////////////////
// Drawing and Display

int Hand::showHand (String name) {
	
	imshow( name, image_binary);
	return 0;
}
int Hand::drawCenterOfMass() {

	/// Put a circle around the the center of mass, radius 50 ///////////
	circle(image_binary, com, 50, (0,255,0), 1);
	return 0;
}
int Hand::drawHandContours(){
	
	Mat src_gray = image_binary;
	RNG rng;
	threshold(src_gray, threshold_output, 200, 255, THRESH_BINARY );
    Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );


    for (int i = 0; i< contours.size(); i++) {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( image_binary, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
       // drawContours( drawing, hullPoint, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
    }

	return 0;
}
int Hand::drawBiggestContour(){

	Mat src_gray = image_binary;
	RNG rng;

	threshold(src_gray, threshold_output, 200, 255, THRESH_BINARY );
    Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );

	for( int i = 0; i < contours.size(); i++ ){
	   if (i == largest_index) {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours  (image_binary, contours, i, color, 2, 8, vector<Vec4i>(), 0, Point() );
	   }
     }

	return 0;
}
int Hand::drawConvexHull(){

	Mat src_gray = image_binary;
	RNG rng;

	threshold(src_gray, threshold_output, 200, 255, THRESH_BINARY );
    Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );

	for (int i = 0; i < contours.size(); i++ ){
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours (image_binary, HullP, i, color, 2, 8, vector<Vec4i>(), 0, Point() );
     }

	return 0;

}