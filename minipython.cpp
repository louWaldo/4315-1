#include <iostream>

using namespace std;

// /*** 
//  * getting started with the c++ based interpriter
// 													***/ 

//  How to run it ./minipython freqnumber.py
int main(int argc, char** argv){
	cout << "You have entered " << argc << " arguments:" << "\n";

	for (int i = 0; i < argc; ++i)
		cout << argv[i] << "\n";
	return 0;
}
