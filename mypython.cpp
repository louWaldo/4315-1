#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stack>


using namespace std;

class Variable {
private:
	string variable_name; //name of the variable 
	int storage_location_number; //position of where the information is stored (value ID)
	int value_stored; //the value stored for this variable
public:
	//constructor
	Variable() {
		variable_name = "name";
		storage_location_number = 9999;
		value_stored = 0;
	}

	//overloaded constructor
	Variable(string name, int id, int val) {
		variable_name = name;
		storage_location_number = id;
		value_stored = val;
	}

	//destructor
	~Variable() {
		variable_name = "N/A";
		storage_location_number = 9999;
		value_stored = 0;
	}

	//mutators for variable name
	void set_variable_name(char* name) { variable_name = name; }
	string get_variable_name() { return variable_name; }

	//mutators for storage number
	void set_storage_location_number(int sln) { storage_location_number = sln; }
	int get_storage_location_number() { return storage_location_number; }

	//mutators for value
	void set_value(int val) { value_stored = val; }
	int get_value() { return value_stored; }
};

class Function {
private:
	string function_name;
	string function_value;
public:
	Function() {
		function_name = "none";
		function_value = "0000";
	}
	~Function() {
		function_name = "none";
		function_value = "0000";
	}
	
	void set_func_name(string name) { function_name = name; }
	string get_func_name() { return function_name; }

	void set_func_value(string val) { function_value = val; }
	string get_func_value() { return function_value; }
};



//from expressions
bool is_global_var(string exp, vector<Variable> vars, int& pos);
bool is_func_var(string exp, vector<Variable> func_vars, int& pos);
int evaluate(string exp, vector<Variable>& vars, vector<Function>& funcs, vector<Variable>& func_vars);

//from ifstatement
bool evaluate_if_statement(string op1, string op2, string cmp);


//from infix to postfix
// Function to convert Infix expression to postfix
string InfixToPostfix(string expression);
// Function to verify whether an operator has higher precedence over other
int HasHigherPrecedence(char operator1, char operator2);
// Function to verify whether a character is operator symbol or not.
bool IsOperator(char C);
// Function to verify whether a character is alphanumeric character (numeric digit) or not.
bool IsOperand(char C);
// Function to verify whether a character is a character letter or not.
bool IsVariable(char var);
// Function to evaluate Postfix expression and return output
string InfixToPostfix(string expression);

//from postfixcalculator
int postfix_calculate(string exp, vector<Variable> vars);
int operation(char op, int op1, int op2);

extern int yylex();
extern int yylineno;
extern char* yytext;

int main() {

	vector<Variable> vars;
	vector<Function> funcs;
	vector<Variable> func_vars;
	vector<string> func_names;
	vector<string> mutated_vars;
	int token; 
	int storage_id = 0;
	bool is_true = false;

	token = yylex();

	while(token) {
		//cout << "[" << token << "][" << yytext << "]" << endl;
		//if token is a print statement
		if(token == 1) {
			string statement = "";
			while(token) {
				if(token == 13) {
					cout << statement << endl;
					break;
				}
				else if(token == 16) {
					//print out anything in quotes and parentheses
					statement = statement + yytext + ' ';
					statement.erase(remove(statement.begin(), statement.end(), '"'), statement.end());
					statement.erase(remove(statement.begin(), statement.end(), ')'), statement.end());
				}
				else if(token == 9) {
					//check the variable data for
					//existing data
					for(int i = 0; i < vars.size(); i++) {
						if(vars[i].get_variable_name() == yytext) {
							statement += to_string(vars[i].get_value());
						}
					}
				}
				else if(token == 2) token = yylex();

				token = yylex();
			}
		}
		else if(token == 3) {
			//variables needed
			stack<string> operands;
			string operand1 = "";
			string operand2 = "";
			string comparator = "";
			string str = "";

			token = yylex(); //skips if statement

			//evaluate if statement
			while(token) {
				//if it is the colon then break
				if(token == 6) break;
				//if it is a indentifier check for
				//in variables
				else if(token == 9) {
					for(int i = 0; i < vars.size(); i++) {
						//if it is then push to stack and break
						if(vars[i].get_variable_name() == yytext) {
							// cout << vars[i].get_value() << endl;
							operands.push(to_string(vars[i].get_value()));
							break;
						}
						//else throw error and break
						else {
							cerr << "error : variable does not exist" << endl;
							break;
						}
					}
				}
				//if integer then push to stack
				else if(token == 10)
					operands.push(yytext);
				//if comparison then store for evaluation
				else if(token == 5)
					comparator = yytext;
				
				token = yylex();
			}

			if(!operands.empty()) {
				operand2 = operands.top(); operands.pop();
				operand1 = operands.top(); operands.pop();
			}

			//evaluate if statement
			is_true = evaluate_if_statement(operand1, operand2, comparator);

			//if the statement is true then return to original program
			//and let it evaluate it itself
			if(is_true == true) {
				token = yylex(); token = yylex();
			}
			else {
				token = yylex(); token = yylex();
				if(token == 15) {
					while(token) {
						if(token == 13) {
							token = yylex();
							break;
						}
						token = yylex();
					}
				}
			}
		}
		//if its a else statement
		else if(token == 4) {
			token = yylex();
			//if statement is true
			if(is_true == true) {
				token = yylex(); token = yylex();
				if(token == 15) {
					while(token) {
						if(token == 13) {
							token = yylex();
							break;
						}
						token = yylex();
					}
				}
			}
			//if statement is false
			else {
				token = yylex(); token = yylex();	
			}
		}
		//if its a function definition
		else if(token == 7) {
			string func_name = "";
			bool func_exists = false;

			token = yylex();
			//get func name
			while(token) {
				if(token == 6) break;
				else if(token == 14) token = yylex();

				func_name += yytext;
				token = yylex();
			}

			for(int i = 0; i < func_names.size(); i++) {
				if(func_names[i] == func_name) {
					func_exists = true;
				}
			}

			if(func_exists == true) {
				cout << "TRUE: Override the function." << endl;
			}
			else {
				//create new function
				Function *newFunc = new Function();
				newFunc->set_func_name(func_name);

				stack<string> operands;
				string operand1;
				string operand2;
				string comparator;

				//find return statement to do get function value
				while(token) {
					//return statement
					if(token == 17) {
						token = yylex(); //get the return

						if(token == 9) {
							for(int i = 0; i < func_vars.size(); i++) {
								if(func_vars[i].get_variable_name() == yytext) {
									newFunc->set_func_value(to_string(func_vars[i].get_value()));
								}
							}
						}

						funcs.push_back(*newFunc);

						break;
					}

					//variables in function
					else if(token == 9) {
						bool exists = false;
						string expression = "";

						//if it does not exist in variables
						//then create one
						if(exists == false) {
							Variable *newVar = new Variable;
							newVar->set_variable_name(yytext);
							newVar->set_storage_location_number(storage_id); storage_id++;

							//get expression
							while(token) {
								//evaluate the expression to push to vector
								if(token == 13) {
									newVar->set_value(evaluate(expression, func_vars, funcs, func_vars));
									break;
								}
								//skip variable name and equal sign
								else if(yytext == newVar->get_variable_name() || token == 2) 
									token = yylex();
								else
									expression += yytext;

								token = yylex();
							}

							func_vars.push_back(*newVar);
						}
						//else if it does exist
						else {
							string mutated_var = yytext;

							while(token) {
								if(token == 13) {
									expression += '\n';
									break;
								}
								else if(yytext == mutated_var || token == 2) 
									token = yylex();
								else
									expression += yytext;

								token = yylex();
							}

							for(int i = 0; i < func_vars.size(); i++) {
								if(func_vars[i].get_variable_name() == mutated_var) {
									func_vars[i].set_value(evaluate(expression, vars, funcs, func_vars));
									break;
								}
							}
						}
					}
					//if statements
					else if(token == 3) {
						//get if statement values
						while(token) {
							//if colon end if statement
							if(token == 6) break;
							//if identifier then push value
							else if(token == 9) {
								for(int i = 0; i < func_vars.size(); i++) {
									if(yytext == func_vars[i].get_variable_name()) {
										operands.push(to_string(func_vars[i].get_value()));
										break;
									}
								}
							}
							//if integer push
							else if(token == 10) {
								operands.push(yytext);
							}
							//if comparator
							else if(token == 5) {
								comparator = yytext;
							}

							token = yylex();
						}

						while(!operands.empty()) {
							operand2 = operands.top(); operands.pop();
							operand1 = operands.top(); operands.pop();
						}
						
						is_true = evaluate_if_statement(operand1, operand2, comparator);

						//TRUE if statement
						if(is_true == true) {
							token = yylex(); token = yylex();
						}
						//FALSE if statement
						else {
							token = yylex(); token = yylex();
							if(token == 15) {
								while(token) {
									if(token == 13) {
										token = yylex();
										break;
									}
									token = yylex();
								}
							}
						}
					}
					//else statement
					else if(token == 4) {
						if(is_true == true) {
							token = yylex(); token = yylex(); token = yylex();
							if(token == 15) {
								token = yylex();
								while(token) {
									if(token == 15) {
										token = yylex();
										break;
									}
									cout << "skip " << yytext << endl;
									token = yylex();
								}
							}
						}
						else {
							token = yylex(); token = yylex();
						}
					}

					token = yylex();
				}
			}
		}
		//if it an identifier or variable
		else if(token == 9) {
				bool exists_var = false;
				string expression = "";

				//if it does not exist in variables
				//then create one
				if(exists_var == false) {
					Variable *newVar = new Variable;
					newVar->set_variable_name(yytext);
					newVar->set_storage_location_number(storage_id); storage_id++;

					//get expression
					while(token) {
						//evaluate the expression to push to vector
						if(token == 13) {
							newVar->set_value(evaluate(expression, vars, funcs, func_vars));
							break;
						}
						//skip variable name and equal sign
						else if(yytext == newVar->get_variable_name() || token == 2) 
							token = yylex();
						else
							expression += yytext;

						token = yylex();
					}
					vars.push_back(*newVar);
				}
				//else if it does exist
				else {
					string mutated_var = yytext;

					while(token) {
						if(token == 13) {
							expression += '\n';
							break;
						}
						else if(yytext == mutated_var || token == 2) 
							token = yylex();
						else
							expression += yytext;

						token = yylex();
					}

					for(int i = 0; i < vars.size(); i++) {
						if(vars[i].get_variable_name() == mutated_var) {
							vars[i].set_value(evaluate(expression, vars, funcs, func_vars));
							break;
						}
					}
				}
		}
		//if comment ignore
		else if(token == 12) {
			while(token) {
				if(token == 13) {
					token = yylex();
					break;
				}
				token = yylex();
			}
		}

		token = yylex();
	}

	return 0;
}



//from expressions ////////////////
bool is_global_var(string exp, vector<Variable> vars, int& pos) {
    for(int i = 0; i < vars.size(); i++) {
        if(exp == vars[i].get_variable_name()) {
            pos = i;
            return true;
            break;
        }
    }
    return false;
}

bool is_func_var(string exp, vector<Variable> func_vars, int& pos) {
    for(int i = 0; i < func_vars.size(); i++) {
        if(exp == func_vars[i].get_variable_name()) {
            pos = i;
            return true;
            break;
        }
    }
    return false;
}

//evaluate expressions
int evaluate(string exp, vector<Variable>& vars, vector<Function>& funcs, vector<Variable>& func_vars) {
    int res = 0;
    int pos = 0;
    bool exists_in_vars = false;
    bool exists_in_func = false;

    if(exp.find("+") != string::npos
    || exp.find("-") != string::npos
    || exp.find("*") != string::npos
    || exp.find("/") != string::npos) {
        exp = InfixToPostfix(exp);
        res = postfix_calculate(exp, vars);
    }
    else if(exp.find("()") != string::npos) {
        for(int i = 0; i < funcs.size(); i++) {
            if(exp == funcs[i].get_func_name()) {
                res = stoi(funcs[i].get_func_value());
            }
        }
    }
    else if(is_global_var(exp, vars, pos) == true)
        res = vars[pos].get_value();
    else if(is_func_var(exp, vars, pos) == true)
        res = func_vars[pos].get_value();
    else 
        res = stoi(exp);

    return res;
}
////////////////////////////////////


//from if statement//
bool evaluate_if_statement(string op1, string op2, string cmp) {
	if(cmp == ">") {
		if(stoi(op1) > stoi(op2)) return true;
		else return false;
	}
	else if(cmp == "<") {
		if(stoi(op1) < stoi(op2)) return true;
		else return false;
	}
	else if(cmp == ">=") {
		if(stoi(op1) >= stoi(op2)) return true;
		else return false;
	}
	else if(cmp == "<=") {
		if(stoi(op1) <= stoi(op2)) return true;
		else return false;
	}
	else if(cmp == "==") {
		if(stoi(op1) == stoi(op2)) return true;
		else return false;
	}
	else if(cmp == "!=") {
		if(stoi(op1) != stoi(op2)) return true;
		else return false;
	}
	else {
		cerr << "error : no comparator" << endl;
		exit(1);
	}
}
/////////////////////////////////////////

//from infixtopostfix//
// Function to evaluate Postfix expression and return output
string InfixToPostfix(string expression)
{
	// Declaring a Stack from Standard template library in C++.
	stack<char> S;
	string temp = "";
	string postfix = ""; // Initialize postfix as empty string.
	for(int i = 0;i< expression.length();i++) {

		// Scanning each character from left.
		// If character is a delimitter, move on.
		if(expression[i] == ' ' || expression[i] == ',') continue;

		// If character is operator, pop two elements from stack, perform operation and push the result back.
		else if(IsOperator(expression[i]))
		{
			postfix += '@';
			while(!S.empty() && S.top() != '(' && HasHigherPrecedence(S.top(),expression[i]))
			{
				postfix = postfix + S.top();
				S.pop();
			}
			S.push(expression[i]);
		}
		// Else if character is an operand
		else if(IsOperand(expression[i]) || IsVariable(expression[i]))
		{	
			postfix = postfix + expression[i];
		}

		else if (expression[i] == '(')
		{
			S.push(expression[i]);
		}

		else if(expression[i] == ')')
		{
			while(!S.empty() && S.top() !=  '(') {
				postfix += S.top();
				S.pop();
			}
			S.pop();
		}
	}

	while(!S.empty()) {
		postfix = postfix + S.top();
		S.pop();
	}

	return postfix;
}

// Function to verify whether a character is english letter or numeric digit.
// We are assuming in this solution that operand will be a single character
bool IsOperand(char C){
	if(C >= '0' && C <= '9') return true;
	return false;
}

// Function to verify whether a character is operator symbol or not.
bool IsOperator(char C) {
	if(C == '+' || C == '-' || C == '*' || C == '/' || C== '$')
		return true;

	return false;
}

bool IsVariable(char var) {
    if(var >= 'a' && var <= 'z') return true;
    else return false;
}

// Function to verify whether an operator is right associative or not.
int IsRightAssociative(char op)
{
	if(op == '$') return true;
	return false;
}

// Function to get weight of an operator. An operator with higher weight will have higher precedence.
int GetOperatorWeight(char op)
{
	int weight = -1;

	switch(op) {
	case '+':
	case '-':
		weight = 1;
		break;
	case '*':
	case '/':
		weight = 2;
		break;
	case '$':
		weight = 3;
		break;
	}

	return weight;
}

// Function to perform an operation and return output.
int HasHigherPrecedence(char op1, char op2)
{
	int op1Weight = GetOperatorWeight(op1);
	int op2Weight = GetOperatorWeight(op2);

	// If operators have equal precedence, return true if they are left associative.
	// return false, if right associative.
	// if operator is left-associative, left one should be given priority.
	if(op1Weight == op2Weight)
	{
		if(IsRightAssociative(op1)) return false;
		else return true;
	}
	return op1Weight > op2Weight ?  true: false;
}
/////////////////////////////////////////



//from postfixcalculator
int postfix_calculate(string exp, vector<Variable> vars) {

    stack<int> s;
    int result = 0;
    string var = "";

    for(int i = 0; i < exp.length(); i++) {
        //if it is a digit
        if(IsOperand(exp[i])) {
            int operand = 0;

            //check if its a long integer
            while(i < exp.length() && IsOperand(exp[i])) {
                operand = (operand * 10) + (exp[i] - '0');
                i++;
            }
            i--;

            s.push(operand);
        }
        else if(IsVariable(exp[i])) {
            var += exp[i];

            if(!IsVariable(exp[i+1])) {
                for(int k = 0; k != vars.size(); k++) {
                    if(var == vars[k].get_variable_name()) {
                        s.push(vars[k].get_value());
                        break;
                    }
                }
                var = "";
            }
        }
        else if(IsOperator(exp[i])) {
            int op1 = s.top(); s.pop();
            int op2 = s.top(); s.pop();

            result = operation(exp[i], op1, op2);

            s.push(result);
        }

    }

    while(!s.empty()) {
        result = s.top(); s.pop();
    }
    
    return result;
}

int operation(char op, int op1, int op2) {
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
////////////////////////////////////////