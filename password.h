#include <opencv\cv.h>
#include <opencv\highgui.h>

using namespace cv;
using namespace std;

class Password {

public: 

	// constructors
	Password();
	// Password(int one, int two, int three, int four);
	Password(int one, int two, int three, int fin1, int fin2, int fin3);

	// Functions
	void Print ();
	int comparePassword (Password *pass1, Password *pass2);

	vector<int> location;
	vector<int> size;

};
