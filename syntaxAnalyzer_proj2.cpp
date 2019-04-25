/*
G={N,T,S,R}

PRODUCTION RULES FOR 1ST ITERATION: p. 116_3.9
G={
	N = {E, T, F}					//Set of Nonterminals
	T = {+, -, *, /, (, ), i}		//Set of Terminals
	S = E							//Starting point
	R = {							//Set of production rules
			E -> E + T | E - T | T
			T -> T * F | T / F | F
			F -> (E) | i
	}
}



PRODUCTION RULES FOR 2ND ITERATION: p. 117_3.13
N = {S, E, T, Q, F, R}
T = {i, =, +, -, *, /, (, ), ep}
S = S
R = {
	// Q is like E'
	// R is like T'
		S -> i = E
		E -> TQ       // E -> TE'
		Q -> +TQ | -TQ | ep
		T -> FR		 // T -> FT'
		R -> *FR | /FR | ep
		F -> (E) | i
}

stack holds the reverse order of production rules
a is incoming token

*/


#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <algorithm>
#include "Lexer.h"

using namespace std;
/*
variables needed:
currChar
*/

enum NonTerminals {
	E = 0,
	Q = 1,
	T = 2,
	R = 3,
	F = 4,
	ERROR = 5
};


//FUNCTION PROTOTYPES:
string test();
bool checkTerminal(string s);
void errorMessage();
int getCharacterCol(char c);
int getProdRow(string s);
char lexConvert(string s);


string test() {
	string userInput;
	cout << "Please input a string to be tested for syntax analysis.\n";
	getline(cin, userInput);
	return userInput;
}

bool checkTerminal(string c) {
	bool flag = false;
	vector<string> terminals{ "+", "-", "*", "/", "(", ")", "i", "$" };
	for (size_t i = 0; i < terminals.size(); i++) {
		if (c == terminals[i]) {
			flag = true;
		}
	}
	return flag;
}

void errorMessage() {
	cout << "ERROR ERROR ERROR.\n";
}

int getCharacterCol(char c) {
	switch (c)
	{
	case '+':
		return 0;
		break;

	case '-':
		return 1;
		break;

	case '*':
		return 2;
		break;

	case '/':
		return 3;
		break;

	case '(':
		return 4;
		break;
	case ')':
		return 5;
		break;
	case 'i':
		return 6;
		break;
	case '$':
		return 7;
		break;
	default:
		cout << "This is a default case for getCharacterCol.\n";
		break;
	}
}

char lexConvert(string s) {
	if (s == "IDENTIFIER" ) {
		return 'i';
	}
}

int getProdRow(string s) {
	
	switch (s[0]) {

	case 'E':
		return 0;
		break;
	case 'Q':
		return 1;
		break;
	case 'T':
		return 2;
		break;
	case 'R':
		return 3;
		break;
	case 'F':
		return 4;
		break;
	default:
		cout << "This is a default case for getProdRow";
		break;
	}
}

string predictiveTable[5][8] = { 
		/* +       -      *      /     (     )     i     $  */
	/*E*/{" ",    " ",   " ",   " ",  "TQ", " ",  "TQ", " "},
	/*Q*/{"+TQ","-TQ",   " ",   " ",   " ", "Z",   " ", "Z"},
	/*T*/{" ",    " ",   " ",   " ",  "FR", " ",  "FR", " "},
	/*R*/{"Z",    "Z", "*FR", "/FR",   " ", "Z",   " ", "Z"},
	/*F*/{" ",    " ",   " ",   " ", "(E)", " ",   "i", " "}
};


int main() {

	//outputting to a file
	string outName = "out.txt";
	ofstream outFile(outName);
	
	//strings to be tested
	string a = "a = b + c";
	string b = "a = b + c;";
	string c = "a = b + c ;"; //perfect case
	string d = "a=b+c";
	string e = "a=b+c;";

	//VARIABLES NEEDED
	//string stringExp = test(); // string that is being tested for syntax
	
	vector<tokenType> tokens; // holds the lexed stringExp
	tokens = lexer(c); // replace with stringExp after testing is done
	stack<string> productionStack; // stack that holds the Production rules to be processed
	int tokenPointer = 0; // to be incremented once the right character has been popped
	//char incomingToken;

	productionStack.push("$");//push $ onto the stack
	c.push_back('$'); // push a $ at the end of the stringExp in this case string c
	productionStack.push("E");//Push the root node in the stack
	
	
	while (productionStack.size() != 0) { //while the stack is not empty
		string topOfStack = productionStack.top();
		tokenType currentToken = tokens[tokenPointer];
		
		//char convertedID = lexConvert(currentToken.lexemeName);//make function that replaces IDENTIFIER TO i
		
		int charCol = getCharacterCol(lexConvert(currentToken.lexemeName));
		int prodRow = getProdRow(topOfStack);

		if (checkTerminal(topOfStack)) { // if terminal
			if (topOfStack == currentToken.lexemeName) { //PROBLEM HERE
				//pop stack and go to next input token
				productionStack.pop();
				tokenPointer++;
			} else {
				errorMessage(); //error
			}
		} else {

			if (predictiveTable[prodRow][charCol] != " ") { // not blank therefore there is a production rule associated on the right side
				string ruleFromTable = " ";
				ruleFromTable = predictiveTable[prodRow][charCol]; //this has the production rule now
				productionStack.pop(); // pops the top of the stack
				reverse(ruleFromTable.begin(), ruleFromTable.end());//reverse the order of the rule inside the table then push it
				
				//cout << "PRINT SOMETHING\n";

				/*for (size_t i = 0; i < ruleFromTable.size(); i++) {
					productionStack.push(ruleFromTable[i]);
				}*/

				
			} else {
				errorMessage();
			}
		}
	}

	return 0;
}


//int main() {
//	//variables needed
//	string userString = test(); // get a string input to be analyzed 
//	vector<tokenType> tokens = lexer(userString);    //vector to hold tokens as they are being inputted
//
//	int tokenIndex = 0;  //Index used to step through token vector
//
//	bool printSwitch = true;
//
//	vector<tokenType> tokenList; //vector that holds all tokens once they have been read in initially
//	tokenType currentToken;
//
//	//emter root node now
//
//	return 0;
//}

//for (size_t i = 0; i < tokens.size(); ++i) {
//	if (tokens[i].lexemeName != "COMMENT") {
//		cout << setw(15) << left << tokens[i].lexemeName << " =" << setw(15) << right << tokens[i].token << endl; // use outFile when done testing
//	}
//
//}