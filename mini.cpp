#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <stack>
#include <algorithm>


using namespace std;

void parser(string exp, vector<string>& variables, vector<int>& values);
void evaluateExp(string exp, vector<string>& var, vector<int>& val);
int HasHigherPrecedence(char op1, char op2);
int getPrec(char op);

bool isNum(char c);
bool isVariable(char c);
bool isOperator(char c);
bool isDelim(char c); // havent used this, but may be useful later, idk
int PostfixCalculate(string postfix, vector<string>& var, vector<int>& val);
int operation(char op, int op1, int op2);

void print(string ins, vector<string> var, vector<int> val);


int main(int argc, char* argv[]) 
{

    ifstream file_in;
    string line;
    vector<string> instructions;
    vector<string> variables;
    vector<int> values;

    file_in.open(argv[1]);

    //instructions = python source code
    //each line is stored in a string, each string in a vector<string>
    while(getline(file_in, line))
    {
        instructions.push_back(line);
    }

    file_in.close();

    for(int i = 0; i != instructions.size(); i++) 
    {
        //if 'print' --> print statement
        if(instructions[i].find("print") != string::npos) 
        {
            //print(string representing instruction, vector of variables, vector of values)
            print(instructions[i], variables, values);
        }

        //if contains "#" or line is empty --> ignore the entire line bc must be comment
        else if(instructions[i].find("#") != string::npos) 
        { 
            continue; 
        }

        //if '=' --> assignment operator
        else if(instructions[i].find("=") != string::npos) 
        {
            //parses line for operators, variables, values, etc, and evaluates any expressions 
            parser(instructions[i], variables, values);
        }

    }

    return 0;
}


////parser functions////
void parser(string exp, vector<string>& variables, vector<int>& values) 
{
    string inString = exp; //temporary container 

    inString.erase(remove(inString.begin(), inString.end(), ' '), inString.end()); //cleans whitespace
    //cout << inString << endl;

    //parses lines into variables and sub expressions
    string var = inString.substr(0, inString.find("="));
    string sub_exp = inString.substr(inString.find("=")+1, inString.size()-1);
    //cout << var << endl;
    //cout << sub_exp << endl;
    variables.push_back(var); //store variables into variable vectpr

    bool isExp = false; //bool to check if expression

    //loop to check if expression
    //must contain an operator
    for(int i = 0; i != sub_exp.length(); i++) 
    {
        if(isOperator(sub_exp[i])) 
        {
           isExp = true;
           break;
        }
        else continue;
    }

    if(isExp == true) 
    {
        //if it is an expression evaluate expression
        evaluateExp(sub_exp, variables, values);
    }
    else 
    {
        //if sub expression is just a string of numbers --> push to value vector
        //stoi to convert string to int
        values.push_back(stoi(sub_exp));
    }

}

//evaluate the expressions
void evaluateExp(string exp, vector<string>& var, vector<int>& val) 
{

    stack<char> s;

    //temporary variables
    string postfix;
    string variable;

    for(int i = 0; i != exp.length(); i++) 
    {
        if(isNum(exp[i]) || isVariable(exp[i])) 
        {
            postfix += exp[i];
        }
        else if(isOperator(exp[i])) 
        {
            //use '%' as separator
            postfix += '%';
            while(!s.empty() && HasHigherPrecedence(s.top(), exp[i])) 
            {
				postfix += s.top();
				s.pop();
			}
			s.push(exp[i]);
        }
    }

    while(!s.empty()) {
        postfix += s.top(); s.pop();
    }

    //PostfixCalculate(postfix, var, val);
    val.push_back(PostfixCalculate(postfix, var, val));

}

// Function to perform an operation and return output.
int HasHigherPrecedence(char op1, char op2) 
{
	int op1Weight = getPrec(op1);
	int op2Weight = getPrec(op2);

	// If operators have equal precedence, return true if they are left associative.
	if(op1Weight == op2Weight) 
    {
        return true;
	}

    if(op1Weight > op2Weight)
    {
        return true;
    }
    return false;
}

// Function to get weight of an operator. An operator with higher weight will have higher precedence.
int getPrec(char op) 
{
	int prec = -1;
	switch(op) {
	case '+':
	case '-':
		prec = 1;
		break;
	case '*':
	case '/':
		prec = 2;
		break;
	}
	return prec;
}



bool isNum(char c) 
{
    //taking advantage of ASCII values for numbers
    if(c >= '0' && c <= '9') return true;
    else return false;
}

bool isVariable(char c) 
{
    //taking advantage of ASCII values for variables
    if(c >= 'a' && c <= 'z') return true;
    else return false;
}

bool isOperator(char c) 
{
    if(c == '+'|| c == '-'|| c == '*'|| c == '/') return true;
    else return false;
}

bool isDelim(char c) 
{
    if(c == ' '|| c == '\n'|| c == '\0') return true;
    else return false;
}


int PostfixCalculate(string postfix, vector<string>& var, vector<int>& val) 
{
    int result = 0; 
    string variable;
    stack<int> s; 

    for(int i = 0; i != postfix.length(); i++) {
        //if a digit then push to stack
        if(isNum(postfix[i])) 
        {
            int operand = 0;
			s.push(operand);
        }
        //if a variable find it from the containers
        else if(isVariable(postfix[i])) 
        {
            variable += postfix[i];

            if(!isVariable(postfix[i+1])) 
            {
                for(int k = 0; k != var.size(); k++) 
                {
                    if(var[k] == variable) 
                    {
                        s.push(val[k]);
                        break;
                    }
                }
                variable = "";
            }
        }
        //if operator then pop two ints and evaluate
        //check for variable still in the expression
        else if(isOperator(postfix[i])) 
        {
            int op1 = s.top(); s.pop();
            int op2 = s.top(); s.pop();
            result = operation(postfix[i], op1, op2);
            //cout << result << endl;
            s.push(result);
        }
    }

    while(!s.empty()) 
    {
        result = s.top();  
        s.pop();
    }

    return result;
}

int operation(char op, int op1, int op2) 
{
    int result = 0;
    switch(op) {
        case '+':
            result = op2 + op1;
            break;
        case '-':
            result = op2 - op1;
            break;
        case '*':
            result = op2 * op1;
            break;
        case '/':
            result = op2 / op1;
            break;
        default:
            break;
    }

    return result;
}




void print(string inString, vector<string> var, vector<int> val) {
    //remove any uneccessary characters
    vector<char> removables = { '(', ')', '"'};

    //parse the line
    string new_line = inString.substr(inString.find('('), inString.find(')'));

    //clean the line
    for(int i = 0; i < removables.size(); i++)
        new_line.erase(remove(new_line.begin(), new_line.end(), removables[i]), new_line.end());

    //check if the single variable call is in the containers already
    bool found = false;
    int pos = 0;
    for(int j = 0; j < var.size(); j++) 
    {
        if(new_line == var[j]) 
        {
            found = true;
            pos = j;
            break;
        }
    }

    if(new_line.find(',') != string::npos) 
    {
        //parse this line now using the ',' delimeter
        string print_line = new_line.substr(0, new_line.find(','));
        string var_line = new_line.substr(new_line.find(',')+1, new_line.size()-1);
        var_line.erase(remove(var_line.begin(), var_line.end(), ' '), var_line.end());

        bool found = false;
        int pos = -1;
        for(int n = 0; n != var.size(); n++) 
        {
            if(var[n] == var_line) 
            {
                found = true;
                pos = n;
                break;
            }
        }

        cout << print_line << val[pos] << endl;

    }
    else if(found == true) 
    {
        cout << val[pos] << endl;
    }
    else
        cout << new_line << endl;
}
