#include <opencv\cv.h>
#include <opencv\highgui.h>
#include "password.h"

using namespace cv;
using namespace std;

// Constructor

Password::Password(){
}
Password::Password(int one, int two, int three, int fin1, int fin2, int fin3){
	location.push_back(one);
	location.push_back(two);
	location.push_back(three);

	size.push_back(fin1);
	size.push_back(fin2);
	size.push_back(fin3);
}

void Password::Print (){
 
  cout << "Quintile Sequence" << endl;
  for (int i = 0; i < location.size(); i++){
    cout << location[i] << "\t";
  }
     cout << endl;

  cout << "Finger Sequence" << endl;
  for (int i = 0; i < location.size(); i++){
    cout << size[i] << "\t";
  }
     cout << endl;
}
int Password::comparePassword (Password *Pass1, Password *Pass2){

	for (int i = 0; i < Pass1->location.size(); i++){
		if (Pass1->location[i] != Pass2->location[i]){
			return 0;
		}
	}

	for (int i = 0; i < Pass1->size.size(); i++){
		if (Pass1->size[i] != Pass2->size[i]){
			return 0;
		}
	}


	return 1;
}