#include <iostream>
#include <fstream>

using namespace std;

// /*** 
//  * getting started with the c++ based interpriter
// 													***/ 

//  How to run it ./minipython freqnumber.py
int main(int argc, char** argv){


	try
    {
        // get the arguments
        if (argc < 2) {
            throw argc;
        } 
        else {
            // Parsing arguments and getting the input and output file names
            string pyfile = argv[1];

			// Starting to parse input file and scrubb
			string Text;
			ifstream inf(pyfile);
			while (getline(inf, Text)) {
				cout << Text << endl;
			}
			inf.close();
			cout << "You have entered " << pyfile << " to be parsed" << "\n";
			return 0;
		}
		
		
	}
    catch(int arguments)
    {
        std::cerr << "Please Give Correct Argument" << '\n' << "Example: ./topText 'input=filename.txt;output=outfile.txt'" << '\n';
    }
    return 0;
}
