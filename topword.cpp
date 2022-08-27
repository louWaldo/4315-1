#include <iostream>
#include <fstream>
using namespace std;
            // "input=f.txt;output=out.txt"
int main(int argc, char** argv) {
    string outfile = argv[1];
    string delimiter = ";";

    size_t pos = 0;
    string infile;
    string outstring;
    while ((pos = outfile.find(delimiter)) != std::string::npos) {
        infile = outfile.substr(0, pos);
        outstring = infile.substr(6);
        outfile.erase(0, pos + delimiter.length());
    }
    string ot = outfile.substr(7);

    string Text;
    ofstream out(ot);
    ifstream inf(outstring);

    while (getline (inf, Text)) {
    // Output the text from the file
        if (Text.length() > 1) {
            out << Text << endl;

        }    




     cout << Text;
}




  return 0;
}