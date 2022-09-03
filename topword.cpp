#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <unordered_map>
using namespace std;

template<typename K, typename V>
void print_map(unordered_map<K, V> const &m, string outname)
{
    ofstream out(outname);
    // find the max value in the map
    V max = 0;
    for (auto const &pair: m) {
        if (pair.second > max) {
            max = pair.second;
        }
    }
    for (auto it = m.cbegin(); it != m.cend(); ++it) {
        // if the first thing is a space or empty word ignore it
        if (it->first[0] == ' ' || it->first.empty()) {
            continue;
        }
        else if (it->second == max) {
            out << (*it).first << " " << (*it).second << endl;
        }
    }
    out.close();
}
//The script is not recursive, so it will read and map the words at the same time.
//The script will read the file and map the words, then it will print the map to a file.
// The Search algorithm for printing is just a simple linear search.
// As there is no need for searching the map, the search algorithm is not optimized.
// The map is not sorted, so the search algorithm is not optimized.
// Since the words are scrubbed, and the scrubb algorithm is on a per word basis there is no high level algorithm
int main(int argc, char** argv) {
    // throw an error is no arguments are passed
    try
    {
        // get the arguments
        if (argc < 2) {
            throw argc;
        } 
        else {
            // Parsing arguments and getting the input and output file names
            string outfile = argv[1];
            string delimiter = ";";
    
            size_t pos = 0;
            string infile;
            string outstring;
            while ((pos = outfile.find(delimiter)) != string::npos) {
                infile = outfile.substr(0, pos);
                outstring = infile.substr(6);
                outfile.erase(0, pos + delimiter.length());
            }
            string ot = outfile.substr(7);
            // Starting to parse input file and scrubb
            string Text;
            unordered_map<string, int> umap;
            ifstream inf(outstring);
            regex e ("\\S*\\d+\\S*");
            regex b ("\\s{2}");
            regex c ("\\W");
            regex d ("[[:punct:]]");
            while (getline (inf, Text)) {
        
            // scrubbing the text
                if (Text.length() > 1) {
                    //scrubbing the text of any unwanted characters/ letter+number combos
                    Text = regex_replace(Text, e, "");
                    Text = regex_replace(Text, b, " ");
                    Text = regex_replace(Text, c, " ");
                    Text = regex_replace(Text, d, " ");
                    string word;
                    //adding the words to the map
                    for (int i = 0; i < Text.length(); i++) {
                        if (Text[i] != ' ') {
                            word += tolower(Text[i]);
                        }
                        else {
                            umap[word]++;
                            word = "";
                        }
                    }
                    if (word != " " && word != "" && umap.find(word) == umap.end() ) {
                        umap[word] = 1;
                    } else {
                        umap[word]++;
                    }
                }
            }
            // print map to file
            print_map(umap,ot);
            inf.close();
        }   
    }
    catch(int arguments)
    {
        std::cerr << "Please Give Correct Argument" << '\n' << "Example: ./topText 'input=filename.txt;output=outfile.txt'" << '\n';
    }
    return 0;
}