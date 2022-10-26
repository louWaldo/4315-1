#include <iostream>
#include <fstream>

#include <iterator>
#include <string>
#include <regex>                                                                   

using namespace std;

// /*** 
//  * getting started with the c++ based interpriter
// 													***/ 

// // 2. tokenize the file
// // 3. parse the file
// // 4. execute the file

// bool is_number(const std::string& s)
// {
//     std::string::const_iterator it = s.begin();
//     while (it != s.end() && std::isdigit(*it)) ++it;
//     return !s.empty() && it == s.end();
// }

// bool is_string(const std::string& s)
// {
//     std::string::const_iterator it = s.begin();
//     while (it != s.end() && std::isalpha(*it)) ++it;
//     return !s.empty() && it == s.end();
// }

// bool is_operator(const std::string& s)
// {
//     std::string::const_iterator it = s.begin();
//     while (it != s.end() && std::isalpha(*it)) ++it;
//     return !s.empty() && it == s.end();
// }

class lexer 
{
    vector<string> tokens;
    public:
        vector<string> tokens(string line){
            // tokenize the line
            //patterns for the regex of : special words, numbers, strings, operators
            string special_words = "if|else|elif|while|for|in|def|return|print|class|import|from|as|pass|break|continue|and|or|not|True|False|None|global|assert|lambda|await";
            string numbers = "[0-9]+";
            string strings = "\".*\"";
            string operators = "[+\\-*/%<>=!&|^~]+";
            string comments = "#.*";
            string whitespace = "\\s+";
            string identifiers = "[a-zA-Z_][a-zA-Z0-9_]*";
            //regex for the patterns
            regex special_words_regex(special_words);
            regex numbers_regex(numbers);
            regex strings_regex(strings);
            regex operators_regex(operators);
            regex comments_regex(comments);
            regex whitespace_regex(whitespace);
            regex identifiers_regex(identifiers);
            //sregex_token_iterator for the regex
            sregex_token_iterator iter(line.begin(), line.end(), special_words_regex, -1);
            sregex_token_iterator end;
            //loop through the iterator and add the tokens to the vector
            for (; iter != end; ++iter) {
                tokens.push_back(*iter);
            }
            
            // return the tokens
            cout << tokens << endl;
            return tokens;
        }

}

// 2. Create a parser
// 3. semantic analysis
// 4. optimization
// 5. code generation

// bool syntax_checkr(){
//     // check for syntax errors
//     return true;
// }





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
