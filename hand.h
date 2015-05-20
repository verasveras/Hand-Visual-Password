#include <opencv\cv.h>
#include <opencv\highgui.h>

using namespace cv;
using namespace std;

class Hand {

public: 

	// constructors
	Hand(); //default constructor
	Hand(Mat image); //constructor


	// calculation and location
	Mat makeBinary(Mat image);
	int centerOfMass();
	int assignLocation();
	int getHeight();
	int getWidth();

	// Contour
	int findHandContours();
	int findBiggestContour();
	int findConvexHull();
	int findContourTop();
	int findContourBottom();
	
	//finger counting
	int fingerBottomCount();
	int fingerTopCount();

	// Retrival
	int getLocationI();
	int getContourArea();
	String getLocationS();
	CvPoint getCenterOfMass();
	vector<vector<Point> > getHullP();
	vector<vector<int> > getHullI();
	vector<vector<Point> > getCountors();


	//Drawing and Display
	int showHand(String name);
	int drawCenterOfMass();
	int drawHandContours();
	int drawBiggestContour();
	int drawConvexHull();


private:

	// image
    Mat image;
	Mat image_binary;

	// Location
	CvPoint com;
	String locationS;
	int locationI;

	int size;
	int height;
	int width;
	int fingers;

	int top;
	int bottom;

	// Contours and ConvexHUll
	vector<vector<Point> > contours;
	vector<vector<Point> > HullP;
	vector<vector<int> > HullI;
	Mat threshold_output;

	// Contour area
	int largest_area;
	int largest_index;

};