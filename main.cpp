#include <opencv\cv.h>
#include <opencv\highgui.h>
#include "hand.h"
#include "password.h"

using namespace cv;
using namespace std;


int main(){


	///////////////////////////////////////////////////////////////////////////////////////
	// Load Images
	Mat im1 = imread("image1.png");
	Mat im2 = imread("image2.png");
	Mat im3 = imread("image3.png");


	if (im1.empty() || im2.empty() || im3.empty()){
        cout << "Cannot load image!" << endl;
        return -1;
    }


	// Create a password.
	// first three ints: locations of hands:
	// last three ints: number of fingers on hands
	Password pass(4,5,5,  5,0,2);

	Hand one(im1);
	one.centerOfMass();
	one.assignLocation();
	CvPoint com1 = one.getCenterOfMass();

	Hand two(im2);
	two.centerOfMass();
	two.assignLocation();
	CvPoint com2 = two.getCenterOfMass();

	Hand three(im3);
	three.centerOfMass();
	three.assignLocation();
	CvPoint com3 = three.getCenterOfMass();

	
	// Find all the contours, draw the biggest contour.
	one.findHandContours();
	one.findBiggestContour();
	one.findContourTop();
	one.findContourBottom();
	int finger1 = one.fingerBottomCount() + one.fingerTopCount() - 1;
	// cout << "Hand One:" << finger1 << endl;
	one.drawBiggestContour();
	one.showHand("Hand One");
	
	two.findHandContours();
	two.findBiggestContour();
	two.findContourTop();
	two.findContourBottom();
	int finger2 = two.fingerBottomCount() + two.fingerTopCount() - 1;
	// cout << "Hand Two:" << finger2 << endl;
	two.drawBiggestContour();
	two.showHand("Hand Two");


	three.findHandContours();
	three.findBiggestContour();
	three.findContourTop();
	three.findContourBottom();
	int finger3 = three.fingerBottomCount() + three.fingerTopCount() - 1;
	// cout << "Hand Three:" << finger3 << endl;
	three.drawBiggestContour();
	three.showHand("Hand Three");


    // Check the password locations and input locatiosn to see if they match.
	Password passCheck(one.getLocationI(), two.getLocationI(), three.getLocationI(), finger1, finger2, finger3);
	cout << "\n---Password---\n" << endl;
	pass.Print();
	cout << "\n---Input Sequence---\n" << endl;
	passCheck.Print();
	
	int result = pass.comparePassword(&pass, &passCheck);
	if (result == 1) cout << "\n\n The input matches the password." << endl;
	else cout << "\n\n The input does not match the password." << endl;


	waitKey(0);
	return 0;
}

