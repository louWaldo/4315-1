#include <iostream>
#include <fstream>
#include <unordered_map>
using namespace std;
            // "input=f.txt;output=out.txt"

template<typename K, typename V>
void print_map(std::unordered_map<K, V> const &m)
{
    for (auto it = m.cbegin(); it != m.cend(); ++it) {
        std::cout << "{" << (*it).first << ": " << (*it).second << "}\n";
    }
}


int main(int argc, char** argv) {
    string outfile = argv[1];
    string delimiter = ";";
    unordered_map<string, int> umap;

    size_t pos = 0;
    string infile;
    string outstring;
    while ((pos = outfile.find(delimiter)) != std::string::npos) {
        infile = outfile.substr(0, pos);
        outstring = infile.substr(6);
        outfile.erase(0, pos + delimiter.length());
    }
    string ot = outfile.substr(7);
//     Your program should display a small “help” if not input parameters are provided and must write error messages to the screen. Your program should not crash, halt unexpectedly or produce unhandled
// exceptions. Consider empty input, blank lines and so on.
    string Text;
    ofstream out(ot);
    ifstream inf(outstring);

    while (getline (inf, Text)) {
    // Output the text from the file
        if (Text.length() > 1) {
            // out << Text << endl;

            // split each Text into words and put them into the unordered_map
            string word;
            size_t pos = 0;
            while ((pos = Text.find(" ")) != std::string::npos) {
                word = Text.substr(0, pos);
                Text.erase(0, pos + 1);
                // out << word << endl;
                if (umap.find(word) == umap.end()) {
                    umap[word] = 1;
                } else {
                    umap[word]++;
                }
            }
            if (umap.find(Text) == umap.end()) {
                umap[Text] = 1;
            } else {
                umap[Text]++;
            }

            print_map(umap);
            cout << umap.size() << endl;
        }
            // clean up logic for text : know rules but your program should handle strange symbols, long/empty lines, and so on.
            // Numbers can be ignored.

    }



  return 0;
}