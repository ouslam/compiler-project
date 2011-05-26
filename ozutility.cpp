/*
 * File Name: ozutility.cpp
 * Author : oz
 */


#include "ozutility.h"

using namespace std;


void scanner::quitCompiler (int ret) {
    infile.close();
	outfile.close();
	retfile.close();
	remove("prog.cpp");
	remove("retaddr.cpp");
	exit(ret);
}

string scanner::converSymType( SymType ft){
	if(ft == INTEGER_SYM){
		return "integer";
	} else if (ft == STRING_SYM){
		return "string";
	} else if (ft == BOOLEAN_SYM) {
		return "boolean";
	} else {
		return "undef";
	}

}

string scanner::convertToken(DETAIL_TOKEN_TYPE t){
	switch (t) {
		case UNDEF: return "UNDEF";
		case MARKER: return "MARKER";
		case FUNCTION : return "FUNCTION";
		case BEGIN: return "BEGIN";
		case END: return "END";
		case IF: return "IF";
		case THEN: return "THEN";
		case ELSE: return "ELSE";
		case WHILE: return "WHILE";
		case CASE: return "CASE";
		case IS: return "IS";
		case WHEN: return "WHEN";
		case DEFAULT: return "DEFAULT";
		case AND: return "AND";
		case OR: return "OR";
		case NOT: return "NOT";
		case NUM:return "NUM";
		case STR:return "STR";
		case ID:return "ID";
		case COMMAR:return "COMMAR";
		case SEMI:return "SEMI";
		case OPARITH:return "OPARITH";
		case OPREL:return "OPREL";
		case OPTERM:return "OPTERM";
		case LP:return "LP";
		case RP:return "RP";
		case LSB: return "LSB";
		case RSB:return "RSB";
		case LB:return "LB";
		case RB:return "RB";
		case ASSIGN:return "ASSIGN";
		default : return "";
	}
}


bool scanner::is_oprand_arith(char c)
{
	if ((c == '+') || (c == '-') || (c =='&') || (c=='|')) 
	{
		//c is oparith
		return true;
	}
	return false;
}

bool scanner::is_oprand_term(char c)
{
	if ((c == '*') || (c == '/') ) 
	{ 
		//c is opterm
		return true;
	}
	return false;
}

bool scanner::is_oprand_e1_relation(char c)
{
	if ((c == '<') || (c == '>') ) 
	{ 
		//c is oprelation
		return true;
	}
	return false;
}

bool scanner::is_oprand_e2_relation(char c)
{
	if ((c == '=') || (c== '!')) {
		//c is oprelation
		return true;
	}
	return false;
}

bool scanner::is_oprand(char c)
{
	// ASCII code 
	// 33 - 34 ! "
	// 38 &
	// 40 - 43 ( ) * +
	// 45 -
	// 47 /
	// 48 - 57 0-9
	// 58 - 62 : ; < = >
	// 65 - 90 A - Z
	// 91 [ 
	// 93 ]
	// 95 _
	// 97 - 122 a-z
	// 123-125 { | }
	if (is_oprand_arith(c) || is_oprand_term(c) || is_oprand_e1_relation(c) ) {
		return true;
	}
	return false;
}

int scanner::parCheck(char c){
	if (c =='(') {
		parStack.push_back(')');
	} else if(c =='[') {
		parStack.push_back(']');
	} else if (c =='{') {
		parStack.push_back('}');
	} else if (c==')' || c == ']' || c== '}') {
		if (parStack.back() == c) {
			parStack.pop_back();
		} else {
			cout << "parenthesis is not matching"<< endl;
			quitCompiler (201);
		}
	} else {
		return 1;
	}
	
	return 0;

}

bool scanner::is_bracket(char c)
{
	if ((c =='(') || (c==')')) {
		return true;
	}
	if ((c =='[') || (c==']')) {
		return true;
	}
	if ((c =='{') || (c=='}')) {
		return true;
	}
	return false;
}

DETAIL_TOKEN_TYPE scanner::get_bracket_type (char c){
	switch (c) {
		case '(': 
			return LP;
		case ')':
			return RP;
		case '[': 
			return LSB;
		case ']':
			return RSB;
		case '{': 
			return LB;
		case '}':
			return RB;
		default :
			return UNDEF;
	}
}

bool scanner::is_valid_id_char(char c){
	if(isalnum(c) || (c == '_')) {
		return true;
	} else {
		return false;
	}
}

bool scanner::is_continued_id_char(char c){
	if(isdigit(c) || (c == '_')) {
		return true;
	} else {
		return false;
	}
}

bool scanner::is_valid_str_char(char c){
	if(isalnum(c) || (c == '_') || (c == ' ')) {
		return true;
	} else {
		return false;
	}
}

/* _ is not accepted as the first char
 * // and / are not differing in the function. // is to be handled in the line processing
 */
int scanner::is_accepted_char(char c)
{
	if(isalpha(c)) {
		return 1;
	}
	if(isdigit(c)) {
		return 2;
	}
	if (is_oprand(c) ){
		return 3;
	}
	if (is_bracket(c)) {
		return 4;
	}
	if (is_oprand_e2_relation(c) ) {
		// a following = is required.
		return 5;
	}
	if (c == '"') {
		// a following " is required.
		return 6;
	}
	if (c == ':' ) {
		// a following = is required.
		return 7;
	}
	if (c== ',') {
		return 8;
	}
	if (c== ';'){
		return 9;
	}
	return 0;
}

DETAIL_TOKEN_TYPE scanner::is_keyword (string word){
	int i=0;
	switch (word[i] ){
		case 'a':
			// and
			if (word.compare("and") == 0) {
				return AND;
			}
			break;
		case 'b':
			// begin
			if (word.compare( "begin") == 0) {
				return BEGIN;
			} else if (word.compare( "boolean") == 0){
				return MARKER;
			}
			break;
		case 'c':
			if (word.compare( "case") == 0) {
				return CASE;
			}
			break;
		case 'd':
			if (word.compare( "default") == 0) {
				return DEFAULT;
			}
			break;
		case 'e':
			if (word.compare( "end") == 0) {
				return END;
			} else if (word.compare( "else") == 0){
				return ELSE;
			}
			break;
		case 'f':
			if (word.compare( "function") == 0) {
				return FUNCTION;
			}
			break;
		case 'i':
			if (word.compare( "if") == 0) {
				return IF;
			} else if (word.compare( "integer") == 0){
				return MARKER;
			} else if (word.compare("is") == 0){
				return IS;
			}
			break;
		case 'n':
			if (word.compare( "not") == 0) {
				return NOT;
			}
			break;
		case 'o':
			if (word.compare( "or") == 0) {
				return OR;
			}
			break;
		case 's':
			if (word.compare( "string") == 0) {
				return MARKER;
			}
			break;
		case 't':
			if (word.compare( "then") == 0) {
				return THEN;
			}
			break;
		case 'w':
			if (word.compare( "while") == 0) {
				return WHILE;
			} else if (word.compare( "when") == 0){
				return WHEN;
			}
			break;
		default:
			return ID;
			break;
	}
	return ID;
}
/*
// Parse out a single token 
int scanner::getToken(string line){
    int i=0;
	int j=0;
	string tmp;

	int length = line.length()-1;
	//cout << "Line Length: " << length << endl;

	while (true) {
		if (i > length) {
			break;
		}
		while(isspace(line[i])){
			if(i== length) {
				// nothing left in the line
				return 1;
			}
			++i;
		}


		// remove comments
		j = i + 1;
		if ((line[i] == '/') && (j <= length) &&(line[j] == '/')){
			break;
		}

		// parse string
		int status = is_accepted_char (line[i]);
		if (status == 1) {
			// alpha
			j=i+1;
			while(isalpha(line[j])) {
				if (j == length){
					tmp = line.substr(i,j-i+1);
					insertToken(is_keyword(tmp), tmp);
					return 1;
				}
				++j;
			}


			if (is_continued_id_char(line[j])){
				++j;
				while(is_valid_id_char(line[j])) {
					if (j == length){
						tmp = line.substr(i,j-i+1);
						insertToken(ID, tmp);
						return 1;
					}
					++j;
				}

				tmp = line.substr(i,j-1-i+1);
				insertToken(ID, tmp);
			} else {
				tmp = line.substr(i,j-1-i+1);
				insertToken(is_keyword(tmp), tmp);

			}
			i=j;
			continue;
		} else if (status == 2){
			// number
			int j=i+1;
			while(isdigit(line[j])){
				if (j == length){
					tmp = line.substr(i,j-i+1);
					insertToken(NUM, tmp);
					return 1;
				}
				++j;
			}
			tmp = line.substr(i,j-1-i+1);
			insertToken(NUM, tmp);
			i=j;
			continue;
			
			
		} else if (status == 3){
			// e1-oprand
			tmp = line.substr(i,1);
			if (tmp.compare(">") ==0 || tmp.compare("<") ==0) {
				insertToken(OPREL, tmp);
			} else if  (tmp.compare("/") ==0 || tmp.compare("*") ==0) { 
				insertToken(OPTERM, tmp);
			} else {
				insertToken(OPARITH, tmp);
			}
			++i;
			continue;
		} else if (status == 4){
			// BRACKET
			tmp = line.substr(i,1);
			parCheck(line[i]);
			insertToken(get_bracket_type(line[i]), tmp);
			++i;
			continue;
		} else if (status == 5){
			// e2-oprand
			int j = i+1;
			if ((j<= length) && line[j] == '='){
				tmp = line.substr(i,j-i+1);
				insertToken(OPREL, tmp);
			} else {
				// throw error
				cout << "Expecting =" << endl << "Error" << endl;
				quitCompiler (101);
			}
			i=j+1;
			continue;
		} else if (status == 6){
			// string
			j=i;
			do {
				++j;
				if (j > length){
					// throw error
					// Error out
					cout << "Expecting \"" << endl << "Error" << endl;
					quitCompiler (102);
					//break;
				}
			}while(is_valid_str_char(line[j]));

			if (line[j] == '"'){
				tmp = line.substr(i,j-i+1);
				insertToken(STR, tmp);
				//cout << "TOKEN : STR" << tmp << endl;
			} else {
				cout << "Expecting \"" << endl << "Error" << endl;
				quitCompiler (103);

				// throw error
				// Error out
			}
			i=j+1;
			continue;
		} else if (status == 7){
			// :=
			int j = i+1;
			//cout << "Assign" << ASSIGN << endl;
			if ((j<= length) && line[j] == '='){
				tmp = line.substr(i,j-i+1);
				insertToken(ASSIGN, tmp);
			} else {
				// throw error
				cout << "Expecting =" << endl << "Error" << endl;
				quitCompiler (104);
			}
			i=j+1;
			continue;
		} else if (status == 8){
			// ,
			tmp = line.substr(i,1);
			insertToken(COMMAR, tmp);
			++i;
			continue;
		} else if (status == 9){
			//;
			tmp = line.substr(i,1);
			insertToken(SEMI, tmp);
			++i;
			continue;
		} else {
			cout << "Expecting char" << endl << "Error" << endl;
			quitCompiler (105);
			// throw error,
			// char is not accepted.
		}

		// get a token

	}

    return 1;

}

*/

PSTATE scanner::get_next_state(DETAIL_TOKEN_TYPE t){
	if(st == S0) {
		if (t == MARKER) {
			return S1;
		} else {
			return SERROR;
		}
	} else if (st == S1) {
		if (t == FUNCTION) {
			return S2;
		} else {
			return SERROR;
		}
	} else if (st == S2) {
		if (t == ID) {
			return S3;
		} else {
			return SERROR;
		}
	} else if (st == S3) {
		if (t == LP) {
			return S4;
		} else {
			return SERROR;
		}
	} else if (st == S4) {
		if (t == MARKER) {
			return S5;
		} else if (t == RP){
			return S11;
		} else {
			return SERROR;
		}
	} else if (st == S5) {
		if (t == ID) {
			return S6;
		} else {
			return SERROR;
		}
	} else if (st == S6) {
		if (t == COMMAR) {
			return S7;
		} else if (t == LSB){
			return S8;
		} else if (t == RP){
			return S11;
		} else {
			return SERROR;
		}
	} else if (st == S7) {
		if (t == MARKER) {
			return S5;
		} else {
			return SERROR;
		}
	} else if (st == S8) {
		if (t == NUM) {
			return S9;
		} else {
			return SERROR;
		}
	} else if (st == S9) {
		if (t == RSB) {
			return S10;
		} else {
			return SERROR;
		}
	} else if (st == S10) {
		if (t == COMMAR) {
			return S7;
		} else if (t == RP){
			return S11;
		} else {
			return SERROR;
		}
	} else if (st == S11) {
		if (t == MARKER) {
			return S12;
		} else if (t == BEGIN){
			return S17;
		} else {
			return SERROR;
		}
	} else if (st == S12) {
		if (t == FUNCTION) {
			return S2;
		} else if (t == ID){
			return S13;
		} else {
			return SERROR;
		}
	} else if (st == S13) {
		if (t == SEMI) {
			return S11;
		} else if (t == LSB){
			return S14;
		} else {
			return SERROR;
		}
	} else if (st == S14) {
		if (t == NUM) {
			return S15;
		} else {
			return SERROR;
		}
	} else if (st == S15) {
		if (t == RSB) {
			return S16;
		} else {
			return SERROR;
		}
	} else if (st == S16) {
		if (t == SEMI) {
			return S11;
		} else {
			return SERROR;
		}
	} else if (st == S17) {
		if (t == ID) {
			return S19;
		} else if (t == IF || t == WHILE || t == CASE){
			return S21;
		} else if (t == WHEN){
			return S23;
		} else if (t == DEFAULT){
			return S24;
		} else if (t == END){
			return S18;
		} else if (t == ELSE) {
			return S17;
		} else {
			return SERROR;
		}
	} else if (st == S18) {
		if (t == FUNCTION) {
			return S26;
		} else if (t == IF || t == WHILE || t == CASE){
			return S25;
		} else {
			return SERROR;
		}
	} else if (st == S19) {
		if (t == LSB || t == ASSIGN) {
			return S21;
		} else {
			return SERROR;
		}
	} else if (st == S20) {
		if (t == END) {
			return S18;
		//} else if (t == SEMI || t == THEN || t == WHILE || t == CASE){
		} else if (t == SEMI || t == THEN){
			return S17;
		} else if (t == WHILE || t == CASE || t == IF || t == ASSIGN ){
			return S21;
		} else if (t == OPARITH || t == OPREL || t == OPTERM || t == AND || t == OR || t == LP || t == LSB || t == COMMAR ){
			return S21;
		} else if (t == RP || t == RSB ){
			return S20;
		} else if (t == ID){
			return S20;
		} else if (t == IS) {
			return S22;
		} else {
			return SERROR;

		}
	} else if (st == S21) {
		if (t == LP || t == COMMAR || t == NOT || t == ASSIGN || t == RP || t == RSB) {
			return S21;
		} else if (t == IS){
			return S22;
		} else if (t == ID || t == NUM || t == STR){
			return S20;
		} else {
			return SERROR;
		}
	} else if (st == S22) {
		if (t == WHEN) {
			return S23;
		} else {
			return SERROR;
		}
	} else if (st == S23) {
		if (t == NUM) {
			return S24;
		} else {
			return SERROR;
		}
	} else if (st == S24) {
		if (t == THEN) {
			return S17;
		} else {
			return SERROR;
		}
	} else if (st == S25) {
		if (t == SEMI) {
			return S17;
		} else {
			return SERROR;
		}
	} else if (st == S26) {
		if (t == SEMI) {
			return S11;
		} else {
			return SERROR;
		}
	}
	return st;
}

int scanner::insertToken(DETAIL_TOKEN_TYPE t, string name){
	PSTATE next;
	next = get_next_state(t);
	if (next == SERROR) {
		cout << "Not Accepted in the state machine" << endl;
		cout << "current state: " << this->st << endl;
		cout << "token :" << t << name << endl;
		cout << "next state :"<< next << endl;
		cout << "line no:" << lineno <<endl;
		quitCompiler (201);
	} else {
		st = next;
		//cout << "Name: " << name ;
		//cout << " Type: " << t ; 
		//cout << " Next State is :" << next << endl;
	}
	token tk;
	tk.type = t;
	tk.name = name;
	tk.lineno = lineno;
	tlist.push_back(tk);
	return 0;

}

int scanner::openFile (string fname) {
	infile.open (fname.c_str());
	if (infile.fail()) {
		cout << "Can't open the file " << fname << endl;
		quitCompiler (101);
	}
	outfile.open ("prog.cpp");
	if (outfile.fail()) {
		cout << "Can't write to the file " << "prog.cpp" << endl;
		quitCompiler (101);
	}
	InitParamFile();

	retfile.open ("retaddr.cpp");
	if (retfile.fail()) {
		cout << "Can't write to the file " << "prog.cpp" << endl;
		quitCompiler (101);
	}

	return 0;
}

int scanner::closeFile() {
	infile.close();
	outfile.close();
	retfile.close();
	GenWrapperFile("wrapper.cpp");
	GenBuildInFuncFile("build_in.cpp");
	GenBuildInLibCPPFile ("func_build_in.cpp");
	GenBuildInLibHFile ("func_build_in.h");
	GenMemHFile("mem.h");
	return 0;
}

/*
int scanner::readFile(string fname){	
// read in programme file by line
	ifstream inFile(fname.c_str(), ios::in);
	string line;

	while(getline(inFile, line)){
		++lineno;
		getToken(line);
		this->lineinfo.push_back(line);
	}
	inFile.close();

	return 0;
}
*/

bool scanner::expectKeyword(DETAIL_TOKEN_TYPE t,unsigned int i,string function,int exitVal){

	if ( ! tryKeyword (t,i,function)) {
		cout << "Error in Function "<< function << endl;
		cout << "Expecting keyword " << convertToken (t) << endl;
		cout << "But run into following "<< endl;
		cout << "Type: "<< convertToken (tlist[i].type ) << endl;
		cout << "Name: " << tlist[i].name << endl;
		cout << "Line:" << lineno << endl;
		quitCompiler(exitVal);
	}
	return true;

}


bool scanner::tryKeyword(DETAIL_TOKEN_TYPE t,unsigned int i,string function){
/*	if (i > tlist.size()) {
		cout << "Out of Bound in function expectKeyword " << endl;
		quitCompiler (99);
	}
*/

	while ( i >= tlist.size()) {
		int ret = parseNextToken();
		if (ret == 0) {
			cout << "Out of Bound in function expectKeyword " << endl;
			quitCompiler (99);
		}
	}
	if (tlist[i].type == t){
		tokenPt = i+1;
		return true;
	} else {
		return false;
	}
}

char scanner::ParseNextChar (int opt) {
	char c = infile.get();
	if (opt == 0){
		if (c >= 'A' && c<= 'Z'){
			c = tolower(c);
		}
	} 
		if (c == '\n') {
			++ lineno;
		}
	return c;
}

void scanner::PutBackNextChar (char c) {
	infile.putback(c);
	if (c == '\n') {
		-- lineno;
	}
}

	
int scanner::parseNextToken(){
	char c;
	int status = 0;
	char c2;

	if (infile.eof()) {
		return 0;
	}
	while (infile.good()) {
		c = ParseNextChar();
		// handling 
		status = is_accepted_char(c);
		if ( status != 0 ) {
			break;
		}
	}
	if (infile.eof()) {
		return 0;
	}
	//cout <<"s: " << status << endl;
	if (c == '/') {
		c2 = ParseNextChar();
		if (c2 == '/') {
			c = ParseNextChar();
			while (!infile.eof() ){
				if (c == '\n') {
					while ( ! infile.eof()) {
						c = ParseNextChar();
						// handling 
						status = is_accepted_char(c);
						if ( status != 0 ) {
							break;
						}
					}
					break;
				}
				c = ParseNextChar(); 
			}
		} else {
			PutBackNextChar(c2);
		}
	}
	//cout <<"s: " << status << endl;
	//cout << "Tag" << endl;;
	string tokenName = "" ;
	tokenName += c;
	//status = is_accepted_char(c);
	//cout << "status: " << status << endl;
	c = ParseNextChar();
		if (status == 1) {
			// alpha
			while ( infile.good() ) {
				if (isalpha( c ) ) {
					tokenName += c;
					c = ParseNextChar();
				} else {
					break;
				}
			}


			if (is_continued_id_char(c)){
				tokenName += c;
				c = ParseNextChar();
				while(is_valid_id_char(c)) {
					tokenName += c;
					c = ParseNextChar(); 
				}
				insertToken(ID, tokenName);
			} else {
				insertToken(is_keyword(tokenName), tokenName);
			}
			PutBackNextChar(c);
			return 1;
		} else if (status == 2){
			// number
			//tokenName += c;
			//c = infile.get();
			//int j=i+1;
			while( isdigit( c ) ){
				tokenName += c;
				c = ParseNextChar();
			}
			insertToken(NUM, tokenName);
			PutBackNextChar(c);
			return 1;
		} else if (status == 3){
			// e1-oprand
			//tmp = line.substr(i,1);
			if ( (tokenName[0] == '>') || ( tokenName[0] == '<') ){
				insertToken(OPREL, tokenName);
			} else if ( (tokenName[0] == '/') || ( tokenName[0] == '*')  ){
				insertToken(OPTERM, tokenName);
			} else {
				insertToken(OPARITH, tokenName);
			}
			PutBackNextChar(c);
			return 1;
		} else if (status == 4){
			// BRACKET
			//tmp = line.substr(i,1);
			parCheck( tokenName[0] );
			insertToken(get_bracket_type(tokenName[0]), tokenName);
			PutBackNextChar(c);
			return 1;
		} else if (status == 5){
			// e2-oprand
			if (c == '=' ) {
				tokenName += c;
				insertToken(OPREL, tokenName);
			} else {
				// throw error
				cout << "A single = is not accepted" << endl;
				cout << "Expecting == " << endl << "Error in line "<< lineno << endl;
				quitCompiler(101);
				//quitCompiler (101);
			}
			return 1;
		} else if (status == 6){
			// string
			while (is_valid_str_char(c)) {
				tokenName += c;
				c = ParseNextChar(1);
			}

			if (c == '"'){
				tokenName += c;
				insertToken(STR, tokenName);
			} else {
				cout << "Expecting \"" << endl << "Error" << endl;
				quitCompiler (103);
				// throw error
				// Error out
			}
			return 1;
		} else if (status == 7){
			// :=
			//cout << "Assign" << ASSIGN << endl;
			if (c == '=') {
				tokenName += c;
				insertToken(ASSIGN, tokenName);
			} else {
				// throw error
				cout << "Expecting =" << endl << "Error" << endl;
				quitCompiler (104);
			}
			return 1;
		} else if (status == 8){
			// ,
			insertToken(COMMAR, tokenName);
			PutBackNextChar(c);
			return 1;
		} else if (status == 9){
			//;
			insertToken(SEMI, tokenName);
			PutBackNextChar(c);
			return 1;
		} else {
			cout << "char: " << c << endl;
			cout << "status: " << status << endl;
			cout << "Expecting char" << endl << "Error" << endl;
			quitCompiler (105);
			// throw error,
			// char is not accepted.
		}



	return 0;
}

string scanner::getTokenName(unsigned int i){
	while ( i >= tlist.size() ){
		parseNextToken();
	}
	return tlist[i].name;
}

DETAIL_TOKEN_TYPE scanner::getTokenType(unsigned int i){
	while ( i >= tlist.size() ){
		parseNextToken();

	}
	return tlist[i].type;
}

int scanner::parseStart(void){
	expectKeyword(MARKER,tokenPt,"parseStart", 318);
	expectKeyword(FUNCTION,tokenPt,"parseStart", 318);
	parseFunction();

	return 0;
}

int scanner::parseFunction (void) {
	parseFunctionHeader();
	parseFunctionBody();

	return 0;
}

int scanner::parseFunctionHeader(void){
//	cout << " Enter parseFunctionHeader" << endl;
//	unsigned int i = tokenPt;

	// pop out return addr. 
//	outfile <<" REG[ REG[1] ] = MEM[ REG[0] ] ;" << endl ;
//	outfile << "REG[0] = REG[0] - 1; "<< endl;
//	outfile << "REG[1] = REG[1] + 1; "<< endl;

	expectKeyword(ID,tokenPt,"parseFunctionHeader", 318);
	//string varName =  tlist[tokenPt-1].name ;
	string varName = getTokenName(tokenPt-1);
	if (symt.func_cur != -1) {
		outfile << "goto " << varName << "_end" <<";"<< endl;
		outfile << varName <<":"<< endl;
	}

	int ret = symt.insertFunctionName(varName);
	if (ret < 0 ) {
		cout << "Function name conflicts in line: " << lineno << endl;
		cout << "Function " << varName << " is already defined" << endl;
		quitCompiler (315);
	}
	list<Symbol>::iterator p ;
	p = symt.insert( symt.func_array[symt.func_cur].name , varName );
	//p->type = symt.convert(tlist[tokenPt-3].name);
	p->type = symt.convert( getTokenName (tokenPt-3) );
	p->cls = FUNCTION_CLASS;
	p->arr = NO_ARRAY;
	p->arrSize = 0 ;
	//p->memPos = memPtr ++;
	p->memPos = 0;
	//p->memPos = symt.func_array[symt.func_cur].stack_num ;
	//++(symt.func_array[symt.func_cur].stack_num);
	//outfile << "REG[1] = REG[1] + 1 ;" << endl;
	p->regPos = regPtr ++;

	//if (func_array[symt.func_cur].parent != -1) {
		// function type for current function is removed after "end function"
		// save a copy of function type for the parent. 
	//	symt.insert(func_array[func_array[symt.func_cur].parent].name,tlist[tokenPt-1].name, symt.convert(tlist[tokenPt-3].name));
	//}
	//cout << "Name: " << func_array[symt.func_cur].name << " Var :" << tlist[tokenPt-1].name << " Value: " << tlist[tokenPt-3].name << endl;
	expectKeyword(LP,tokenPt,"parseFunctionHeader", 318);
	parseParameterList(0);

	return 0;
}

int scanner::parseArraySize(void){
	expectKeyword(NUM,tokenPt,"parseArraySize", 318);
	expectKeyword(RSB,tokenPt,"parseArraySize", 318);
	//cout << "array size: " <<tlist[tokenPt-2].name << endl;
	//return atoi(tlist[tokenPt-2].name.c_str());
	return atoi( getTokenName(tokenPt-2).c_str() );
}


int scanner::parseParameter( int index ){
	expectKeyword(MARKER,tokenPt,"parseParameter", 318);
	expectKeyword(ID,tokenPt,"parseParameter", 318);
	
	//string varName = tlist[tokenPt-1].name;
	//string varType = tlist[tokenPt-2].name;
	string varName = getTokenName(tokenPt-1); 
	string varType = getTokenName(tokenPt-2); 

	list<Symbol>::iterator p ;
	p = symt.insert(symt.func_array[symt.func_cur].name, varName );
	p->type = symt.convert(varType);
	p->cls = PAR_CLASS;
	//cout << "Name: " << func_array[symt.func_cur].name << " Var :" << tlist[tokenPt-1].name << " Value: " << tlist[tokenPt-2].name << endl;
	if ( tryKeyword(LSB,tokenPt,"parseParameter")) {
		// TBD : array parameter feature not supported. 
		int size = parseArraySize();
		p->arr = YES_ARRAY ;
		p->arrSize = size;
		symt.insertFuncPar (symt.func_array[symt.func_cur].name,varName, symt.convert(varType), PAR_CLASS, YES_ARRAY);
	} else {
		//p->memPos = memPtr++;
		//if ( symt.func_array[symt.func_cur].parent != -1 ) {
			// pop stack
			//p->regPos = popStack();
			//p->regPos = regPtr++; 
			//p->regPos = getStackValue(index + 2);
			//outfile << "MEM[ REG[1] + " << index <<"] = REG[" << p->regPos <<"] ;" << endl; 
			//cout << "pop out in-order parameter: "<< index << endl;
		//} else {
			p->regPos = regPtr++; 
			//outfile << "REG[" << p->regPos <<"] = MEM[" << memPtr++ <<"] ;" << endl;
			//outfile << "REG[1] = REG[1] + 1 ;" << endl;
			
		//}
		p->arr = NO_ARRAY;
		p->arrSize = 0;
		symt.insertFuncPar (symt.func_array[symt.func_cur].name,varName, symt.convert(varType), PAR_CLASS, NO_ARRAY);
//		p->memPos = index + 2;
		p->memPos = symt.func_array[symt.func_cur].stack_num;
		++(symt.func_array[symt.func_cur].stack_num);
//		p->regPos = regPtr ; 
		//cout << "INFO " << varName << " is REG[" <<p->regPos  <<"] ;" << endl;
	}

	return 0;

}

int scanner::parseParameterList( int index ){
	if (tryKeyword (RP, tokenPt,"parseParameterList" )) {
		return 0;
	}
	parseParameter( index );
	++ index;
	if (tryKeyword (COMMAR, tokenPt,"parseParameterList" )){
		parseParameterList(index );
	} else if (tryKeyword (RP, tokenPt,"parseParameterList" )){
		return 0;
	} else {
		cout << "Error in parseParameterList" << endl;
		cout << "Expecting ID or FUNCTION" << endl;
		quitCompiler (315);
	}


	return 0;

}

int scanner::parseLineDeclaration( void ){
	if (tryKeyword (FUNCTION, tokenPt,"parseLineDeclaration" )){
		parseFunction();
	} else if (tryKeyword (ID, tokenPt,"parseLineDeclaration" )){

//		cout << "Insert syn table " << endl;
		list<Symbol>::iterator p ;
		p = symt.insert(symt.func_array[symt.func_cur].name, getTokenName(tokenPt-1) );
		p->type = symt.convert( getTokenName(tokenPt-2) ) ;
//		cout << "Insert syn table " << endl;
		p->cls = VAR_CLASS;
		//cout << "Name: " << func_array[symt.func_cur].name << " Var :" << tlist[tokenPt-1].name << " Value: " << tlist[tokenPt-2].name << endl;
		//cout << "MEM["<< memPtr <<"] is allocated for variable " << tlist[tokenPt-1].name << endl;
		if (tryKeyword (LSB, tokenPt,"parseLineDeclaration" )){
			p->arr = YES_ARRAY;
			p->arrSize = parseArraySize();
			p->regPos = regPtr;
			if (symt.func_array[symt.func_cur].parent == -1) {
				p->memPos = memPtr;
				memPtr+= p->arrSize;
			} else {
				p->memPos = symt.func_array[symt.func_cur].stack_num ;
				symt.func_array[symt.func_cur].stack_num += p->arrSize ;
			}
			//regPtr  += p->arrSize;
			++regPtr ;
			//memPtr += p->arrSize;
			//cout << "INFO: " << tlist[tokenPt-2].name << " is REG[" << p->regPos<< "]" << endl;
			//for (int i = 0; i< p->arrSize; ++i) {
			//	outfile << "REG["<<p->regPos+i<<"] = MEM["<< p->memPos+i <<"];" << endl;
			//}
			//outfile << "REG[1] = REG[1] + "<< p->arrSize <<" ;" << endl;
		} else {
			p->arr = NO_ARRAY;
			p->arrSize = 0;
			p->regPos = regPtr ;
			if (symt.func_array[symt.func_cur].parent == -1) {
				p->memPos = memPtr++;
			} else {
				p->memPos = symt.func_array[symt.func_cur].stack_num ;
				++ (symt.func_array[symt.func_cur].stack_num) ;
			}
			++ regPtr;
			//++ memPtr;
			//cout << "INFO: " << tlist[tokenPt-1].name << " is REG[" << p->regPos<< "]" << endl;
			//outfile<< "REG["<< p->regPos <<"] = MEM["<< p->memPos <<"];" << endl;
			//outfile << "REG[1] = REG[1] + 1 ;" << endl;
		}
	} else {
		cout << "Error in parseLineDeclaration" << endl;
		cout << "Expecting ID or FUNCTION" << endl;
		quitCompiler (315);
	}

	return 0;
}


int scanner::parseFuncBodyDeclaration( void ){

	while (tryKeyword (MARKER, tokenPt, "parseFuncBodyDeclaration")){
		parseLineDeclaration();
		expectKeyword (SEMI, tokenPt, "parseFuncBodyDeclaration", 315);
	} 
	
	return 0;
}



int scanner::parseFunctionBody(void){
	parseFuncBodyDeclaration();
	expectKeyword (BEGIN, tokenPt, "parseFunctionBody", 315);
	parseStatement();
	expectKeyword (END, tokenPt, "parseFunctionBody", 315);
	expectKeyword (FUNCTION, tokenPt, "parseFunctionBody", 315);
	// pop out local variable. 
//	outfile << "REG[0] = REG[0] - 1; "<< endl;

	// DONOT use push/pop operation because it is inorder parsing. 


	int func_tmp = symt.func_cur;
	symt.func_cur = symt.getParentFunction();

	// pop out function parameter
	//int i = symt.func_par_table[ symt.func_array[func_tmp].name ].size() - 2 ;
	//outfile << "REG[0] = REG[0] + " <<i <<"; " << endl;


	// push return variable. 
	if (symt.func_cur != -1) {
		//int retREG = retAddr[symt.func_array[func_tmp].name];
		//outfile << "goto *retAddr[ REG[ "<< retREG <<"] ];"<< endl;
		outfile << symt.func_array[func_tmp].name <<"_end:" << endl;
//	cout << "Function" << func_array[func_tmp].name<< " is done" << endl;
//	symt.remove(func_array[symt.func_cur].name);
//	symt.func_cur = getParentFunction();
	} else {
		// gen addr table
		retfile <<   "void *retAddr["<< retAddrTable.size() + 1 <<"] = { " << endl;
		for (vector <string>::const_iterator p =  retAddrTable.begin(); p!= retAddrTable.end(); ++ p) {
			retfile << "  &&" << *p << "," << endl;
		}
		retfile << "};"<< endl;
	}
	return 0;
}

int scanner::parseLineStatement(void){
	SymType ft1;
	SymType ft2;
	if ( tryKeyword (ID, tokenPt, "parseLineStatement") ) {
		//cout << "ID:" << tlist[tokenPt-1].name << endl;
		string varName = getTokenName(tokenPt-1);
		list<Symbol>::iterator p ; 
		p = symt.getSymbolPointer(varName, symt.func_array[symt.func_cur].name);
		//cout << "Name: "<< tlist[tokenPt-1].name << endl;
		ft1 = p->type;

		int arrayIndex = 0;
		int regINC;
	// int regRET = regPtr++;
	// typechecking here.
		if ( tryKeyword (LSB, tokenPt, "parseLineStatement") ) {
			regINC = regPtr++;
			parseExpression(6, regINC);
		//regNum += regINC;
		//cout << "REG["<< regNum <<"] = REG["<< regINC <<"] +REG[" << regNum<<"]" << endl;
			expectKeyword (RSB, tokenPt, "parseLineStatement",315);
			arrayIndex = 1;
	//} else {
	//	outfile << "REG[" << regINC << "] = 0 ;" << endl;
		}
		expectKeyword (ASSIGN, tokenPt, "parseLineStatement", 315);

		ft2 = parseAssignStatement(p->regPos);
		//cout << "REG["<< p->regPos <<"] = " << endl;
		if (varName.compare(symt.func_array[symt.func_cur].name) == 0 ) {
			if ( symt.func_array[symt.func_cur].parent != -1 ) {
				// pop return value 
				int regNum = regPtr++;
				outfile << "REG[0] = REG[0] + 1;"<<endl;
				outfile << "REG[ "<< regNum <<" ] = MEM[ REG[0] ];"<<endl;
	
				// pop out parameter
				//int i = symt.func_par_table[ symt.func_array[symt.func_cur].name ].size();
				outfile << "REG[0] = REG[0] + " << symt.func_array[symt.func_cur].stack_num - 1  <<"; " << endl;
				// push return value
				pushStack(p->regPos);
				//cout << "push return value " << endl;
				
				outfile << "goto *retAddr[ REG[ "<< regNum <<"] ];"<< endl;
				//outfile << symt.func_array[symt.func_cur].name <<"_end:" << endl;
			}
		// wrong because it could be a[1] but [1] is not parsed out yet.
		} else {
			if ( symt.func_array[symt.func_cur].parent == -1 ) {
				if (arrayIndex == 1) {
					int regTMP = regPtr ++;
					outfile  << " REG["<<regTMP<<"] = "<< p->memPos<<" + REG["<<regINC<<"] ;" << endl;
					outfile  << " MEM [REG [ "<<regTMP<<"]] = REG["<<p->regPos<<"] ;" << endl;
					//outfile  << "MEM[ " << p->memPos <<" + REG["<< regINC<< "] ] = REG[" << p->regPos <<"];" << endl;
				} else {
					outfile  << "MEM[ " << p->memPos <<" ] = REG[" << p->regPos <<"];" << endl;
				//outfile << "MEM[" << p->memPos <<"] = REG[" << p->regPos << "];" << endl;
				}
			} else {
				int distance = 0;
				list<Symbol>::iterator p = symt.getSymbolPointer(varName, symt.func_array[symt.func_cur].name);
				// if the variable is coming from other function
				if ((p->func).compare( symt.func_array[symt.func_cur].name) != 0 ) {
					//ut << "Catch one" << endl;
					//it (2);
					int func_tmp = symt.func_cur;
					distance += symt.func_array[func_tmp].stack_num ;
					while ( (p->func).compare( symt.func_array[func_tmp].name) != 0  ) {
						func_tmp = symt.func_array[func_tmp].parent;
						distance += symt.func_array[func_tmp].stack_num ;
					}
					if (symt.func_array[func_tmp].parent == -1) {
						distance = 0;
					}
				} else {
					distance = symt.func_array[symt.func_cur].stack_num ;
				}
				if (distance == 0) {
					if (arrayIndex == 1) {
						int regTMP = regPtr ++;
						outfile  << "REG["<<regTMP<<"] = "<<p->memPos<<" + REG["<<regINC<<"] ;" << endl;
						outfile  << "MEM[ REG["<< regTMP << "] ] = REG[" << p->regPos <<"];" << endl;
						//outfile  << "MEM[ " << p->memPos <<" + REG["<< regINC<< "] ] = REG[" << p->regPos <<"];" << endl;
					} else {
						outfile  << "MEM[ " << p->memPos <<" ] = REG[" << p->regPos <<"];" << endl;
						//outfile << "MEM[" << p->memPos <<"] = REG[" << p->regPos << "];" << endl;
					}
				} else {
					int gap = distance + 1 - p->memPos;
					if (arrayIndex == 1) {
						int regTMP = regPtr ++;
						outfile  << "REG[" << regTMP << "] = REG[0] + " << gap <<" ; " << endl;
						outfile  << "REG[ "<<regTMP<<" ] = REG[ "<<regTMP <<" ] - REG["<<regINC<<"] ;" << endl;
						outfile  << "MEM[ REG[ " << regTMP <<" ] ] = REG[" << p->regPos <<"];" << endl;
						//outfile  << "MEM[ REG[0] + " << gap <<" - REG["<< regINC<< "] ] = REG[" << p->regPos <<"];" << endl;
					} else {
						int regTMP = regPtr ++;
						outfile  << "REG["<<regTMP<<"] = REG[0] + "<< gap <<" ;" << endl;
						outfile  << "MEM[ REG[ "<<regTMP<<"] ] = REG[" << p->regPos <<"];" << endl;
						//outfile  << "MEM[ REG[0] + " << gap <<" ] = REG[" << p->regPos <<"];" << endl;
						//outfile << "MEM[" << p->memPos <<"] = REG[" << p->regPos << "];" << endl;
					}
				}
			}
		}
		//if ((ft1 == INTEGER_SYM) && (ft2 == BOOLEAN_SYM)) {
		//} else if ((ft2 == INTEGER_SYM) && (ft1 == BOOLEAN_SYM)) {
		//} else if (ft1 != ft2) {
		if (ft1 != ft2) {
			cout << "Error in typechecking in line " << lineno << endl;
			cout << "function: parseLineStatement" << endl;
			cout << "mismatch between des:" << converSymType(ft1) << " And expression: " << converSymType(ft2) << endl;
			quitCompiler(315);
		}
		return 1;
	} else if ( tryKeyword (IF, tokenPt, "parseLineStatement") ) {
		//cout << "IF" << endl;
		parseIfStatement();
		return 2;
	} else if ( tryKeyword (WHILE, tokenPt, "parseLineStatement") ) {
		//cout << "WHILE" << endl;
		parseWhileStatement();
		return 3;
	} else if ( tryKeyword (CASE, tokenPt, "parseLineStatement") ) {
		//cout << "CASE" << endl;
		parseCaseStatement();
		return 4;
	//} else if (tlist[tokenPt].type == END ){
	//	return 0;
	//} else {
	//	cout << "unexpected statement" << endl;
	//	cout << "Name: " << tlist[tokenPt].name << endl;
	//	cout << "Type: " << tlist[tokenPt].type << endl;
	//	quitCompiler (315);
	}
	return 0;

}

int scanner::parseStatement(){

	while (parseLineStatement()!= 0){
		//cout << "New Line" << endl;
		parseStatement();
	}

	return 0;
}

SymType scanner::parseAssignStatement(int regNum){
	SymType ft;
	//parseNextToken();
	//ft = parseExpression(6, regRET);
	//ft = parseLongExpression(regRET);
	ft = parseLongExpression(regNum);
	//if (arrayIndex == 1) {
	//	outfile  << "MEM["<< memNum <<"+ REG["<< regINC<< "] ] = REG[" << regNum<<"];" << endl;
		//outfile  << "REG["<< regNum <<"+ REG["<< regINC<< "] ] = REG[" << regRET<<"];" << endl;
	//} else {
	//	outfile  << "MEM["<< memNum <<" ] = REG[" << regNum<<"];" << endl;
		//outfile  << "REG["<< regNum <<" ] = REG[" << regRET<<"];" << endl;
	//}
	
	expectKeyword (SEMI, tokenPt, "parseAssignStatement", 315);

	return ft;
}

int scanner::parseIfStatement(void){
	int regNum = regPtr++;
	SymType ft1;
	ft1 = parseExpression(6, regNum);
	//if (ft1 != BOOLEAN_SYM && ft1 != INTEGER_SYM) {
	if (ft1 != BOOLEAN_SYM ) {
		//cout << ft1 << endl;
		cout << "keyword \"if\" is not followed by boolean expression " << endl;
		cout << "type checking error in if statement in line " << lineno << endl;
		quitCompiler (315);
	}

	int labelNum = labelIfPtr++ ;
	outfile<< "if ( REG["<< regNum<< "]) goto label_if_"<< labelNum<<"_inside;" << endl;
	outfile<< "goto label_if_"<< labelNum<<"_jump_out;" << endl;
	outfile<< "label_if_"<< labelNum<<"_inside:" << endl;
	expectKeyword (THEN, tokenPt, "parseIfStatement", 315);
	parseStatement();
	outfile<< "goto label_if_"<< labelNum<<"_end;" << endl;
	outfile<< " label_if_"<< labelNum<<"_jump_out: " << endl;

	if ( tryKeyword (ELSE, tokenPt, "parseIfStatement") ) {
		parseStatement();
	}
	outfile<< "label_if_"<< labelNum<<"_end : " << endl;
	expectKeyword (END, tokenPt, "parseIfStatement", 315);
	expectKeyword (IF, tokenPt, "parseIfStatement", 315);
	expectKeyword (SEMI, tokenPt, "parseIfStatement", 315);
//	++labelIfPtr ;

	return 0;
}

int scanner::parseWhileStatement(){
	int regNum = regPtr++;
	//cout << getTokenName(tokenPt - 1) << endl;

	outfile<< "while_" <<labelWhilePtr<<"_loop_start: " << endl;
	SymType ft1;
	ft1 = parseExpression(6, regNum);
	//if (ft1 != BOOLEAN_SYM && ft1 != INTEGER_SYM) {
	if (ft1 != BOOLEAN_SYM ) {
		int k = tlist[tokenPt-1].lineno;
		cout << "type checking error in while statement in line " << k << endl;
		quitCompiler (315);
	}
	outfile<< "if (REG["<<regNum<<"]) goto while_" <<labelWhilePtr<<"_loop;" << endl;
	outfile<< "goto out_while_" <<labelWhilePtr<<"_loop;" << endl;;
	outfile<< "while_" <<labelWhilePtr<<"_loop:" << endl;
	parseStatement();
	outfile << "goto while_" <<labelWhilePtr <<"_loop_start;" << endl;
	outfile<< "out_while_" <<labelWhilePtr <<"_loop:" << endl;

	expectKeyword (END, tokenPt, "parseWhileStatement", 315);
	expectKeyword (WHILE, tokenPt, "parseWhileStatement", 315);
	expectKeyword (SEMI, tokenPt, "parseWhileStatement", 315);
	++ labelWhilePtr;
	return 0;
}

int scanner::parseCaseStatement(){
	int labelCaseBranchPtr = 0;
	int regNum = regPtr++;
	SymType ft1;

	ft1 = parseExpression(6, regNum);
	if (ft1 != INTEGER_SYM) {
		cout << "keyword \"case\" is not followed by integer expression" << endl;
		cout << "type checking error in case statement in line " << lineno << endl;
		quitCompiler (315);
	}
	expectKeyword (IS, tokenPt, "parseCaseStatement", 315);
	while (tryKeyword (WHEN, tokenPt, "parseCaseStatement")){
		expectKeyword (NUM, tokenPt, "parseCaseStatement", 315);
		int regCond = regPtr++;
		outfile << "REG["<<regCond<<"] = (REG["<<regNum<<"] == "<< getTokenName(tokenPt-1) <<");" << endl;
		outfile << "if ( REG["<<regCond<<"] ) goto case_"<< labelCasePtr <<"_branch_"<<labelCaseBranchPtr<< ";" << endl;
		outfile << "goto case_"<< labelCasePtr <<"_branch_"<<labelCaseBranchPtr<< "_end;" << endl;
		outfile << "case_"<< labelCasePtr <<"_branch_"<<labelCaseBranchPtr << ":" << endl;
		expectKeyword (THEN, tokenPt, "parseCaseStatement", 315);
		parseStatement();
		outfile << "goto case_"<< labelCasePtr <<"_end;" << endl;
		outfile << "case_"<< labelCasePtr <<"_branch_"<<labelCaseBranchPtr<< "_end:" << endl;
		++labelCaseBranchPtr;
	}
	outfile << "case_"<< labelCasePtr <<"_default:" << endl;
	if (tryKeyword (DEFAULT, tokenPt, "parseCaseStatement")){
		expectKeyword (THEN, tokenPt, "parseCaseStatement", 315);
		parseStatement();
	}
	outfile << "case_"<< labelCasePtr <<"_end:" << endl;
	expectKeyword (END, tokenPt, "parseCaseStatement", 315);
	expectKeyword (CASE, tokenPt, "parseCaseStatement", 315);
	expectKeyword (SEMI, tokenPt, "parseCaseStatement", 315);
	++ labelCasePtr;

	return 0;
}




SymType scanner::parseLeftExpression(int tag, int retREG){

	int regNum = regPtr ++;
	unsigned int i=tokenPt;
	SymType ft;
	DETAIL_TOKEN_TYPE t;
	string varName;
//	* done = 1;
//	cout << "tag:" << tag << " : "<< tlist[i].name << endl;

	// number
	//parseNextToken();
	t = getTokenType(i);
	varName = getTokenName(i);
	if (tag >= 6 && (t == AND || t == OR)) {
		tokenPt = i+1;
		ft = parseExpression(6, regNum);
		outfile <<"REG[" <<retREG <<"] = REG[" <<retREG<<"] "<< varName<<" REG["<< regNum <<"] ;" << endl;
		if (ft == BOOLEAN_SYM || ft == UNDEF_SYM) {
			return BOOLEAN_SYM;
		} else {
			cout << "Error in TypeChecking" << endl;
			cout << "parseLeftExpression - AND/OR" << endl;
			cout << "Boolean variable with " <<varName << endl;
			quitCompiler (315);
		}
	} else if ( tag >=5 &&  t == OPARITH ) {
		tokenPt = i+1;
		ft = parseExpression(5, regNum);
		outfile <<"REG[" <<retREG <<"] = REG[" <<retREG<<"] "<< varName<<" REG["<< regNum <<"] ;" << endl;
		if (ft == INTEGER_SYM || ft == UNDEF_SYM) {
			return INTEGER_SYM;
		//} else if (ft == BOOLEAN_SYM) {
			// fix int + int > int
		//	return INTEGER_SYM;
		} else {
			cout << "Error in TypeChecking in line " << lineno << endl;
			cout << "parseLeftExpression - ARITH" << endl;
			cout << "Integer variable " << varName << " with " << converSymType(ft) << endl;
			quitCompiler (315);
		}
	} else if ( tag >=4 && t == OPREL ){
		tokenPt = i+1;
		ft = parseExpression(4, regNum);
		outfile <<"REG[" <<retREG <<"] = REG[" <<retREG<<"] "<< varName<<" REG["<< regNum <<"] ;" << endl;
		if (ft == INTEGER_SYM || ft == BOOLEAN_SYM|| ft == UNDEF_SYM) {
			return  BOOLEAN_SYM;
		} else {
			cout << "Error in TypeChecking" << endl;
			cout << "parseLeftExpression - REL" << endl;
			cout << "Int/Bool variable with" << varName <<endl;
			quitCompiler (315);
		}
	} else if ( tag >=3 &&  t == OPTERM) {
		//cout << "inside term "<< endl;
		tokenPt = i+1;
		ft = parseExpression(3, regNum);
		outfile <<"REG[" << retREG <<"] = REG[" <<retREG<<"] "<< varName <<" REG["<< regNum <<"] ;" << endl;
		if (ft == INTEGER_SYM || ft == UNDEF_SYM) {
			return INTEGER_SYM;
		} else {
			cout << "Error in TypeChecking" << endl;
			cout << "parseLeftExpression - TERM" << endl;
			cout << "Integer variable with "  << varName <<endl;
			quitCompiler (315);
		}
	//} else {
	//	cout <<"REG[" <<retREG <<"] is nothing "<< endl;
	}

	return UNDEF_SYM;
}


SymType scanner::parseLongExpression( int retREG ){
//	unsigned int i;
	SymType ft1;
	SymType ft2;
	ft1 = parseExpression(6, retREG);

	DETAIL_TOKEN_TYPE t = getTokenType (tokenPt);
	string varName = getTokenName(tokenPt);
	if (t == AND || t == OR || t == OPARITH || t == OPREL || t == OPTERM) {
		tokenPt += 1;
		//parseNextToken();
		int regTMP = regPtr++;
		//cout << "regTMP: " << regTMP << endl;
		ft2 = parseLongExpression(regTMP);
		outfile << "SPE: REG["<< retREG<<"] = REG["<<retREG<<"] "<< varName <<" REG["<< regTMP<<"] ;"  << endl;
		if (ft1 == ft2) {
			return ft1;
		} else {
			cout << "Error in Type checking" << endl;
			cout << "in parseLongExpression" << endl;
			quitCompiler (315);
		}
	}
	// TBD
	return ft1;
}



// tag <= 6 Expression
// tag <= 5 ArithExpression
// tag <= 4 RelExpression
// tag <= 3 TermExpression

// generate output register code

SymType scanner::parseExpression(int tag, int retREG ){
	unsigned int i=tokenPt;
	SymType ft1;
	SymType ft2;
	//parseNextToken();
	DETAIL_TOKEN_TYPE t = getTokenType (tokenPt);
	string varName =  getTokenName(i);
	if (t == LP) {
		tokenPt = i+1;
		ft1 = parseExpression(6, retREG);
		i = tokenPt;
		if (getTokenType( tokenPt ) == RP){
			tokenPt = i+1;
			//parseNextToken();
			ft2 = parseLeftExpression(tag,retREG);
		} else {
			cout << "Error in parseExpression" << endl;
			cout << "No right bracket" << endl;
			quitCompiler (313);
		}
		if (ft1 == ft2 || ft2 == UNDEF_SYM){
			return ft1;
		} else if (ft1 == INTEGER_SYM && ft2 == BOOLEAN_SYM) {
			// potential error TBD
			return ft2;
		} else {
			cout << "Error in Type checking" << endl;
			cout << "After Bracket" << endl;
			cout << "Mismatch between " << ft1 << " And" << ft2 << endl;
			quitCompiler (315);
		}
	} else if (tag >= 6 && t == NOT){
		tokenPt = i+1;
		int regTMP = regPtr ++;
		ft1 = parseExpression(6, regTMP);
		outfile << "REG["  << retREG <<"] = not REG["<< regTMP <<"] ;" << endl;
		if (ft1 == BOOLEAN_SYM || ft1 == UNDEF_SYM) {
			return ft1;
		} else {
			cout << "Error in Type checking" << endl;
			cout << "After not" << endl;
			quitCompiler (315);
		}
	} else if (t == NUM){
		//DETAIL_TOKEN_TYPE opType = tlist[i+1].type;
		tokenPt = i+1;
		outfile << "REG[" <<retREG <<"] = "<< varName << ";"<< endl;
		ft2 = parseLeftExpression(tag, retREG);
		//cout << "TEST :"<< tlist[i].name << ft2 << endl;
		if (ft2 == UNDEF_SYM ) {
			return INTEGER_SYM;
		} else if (INTEGER_SYM == ft2 || ft2 == BOOLEAN_SYM ){
		//} else if ( ft2 == BOOLEAN_SYM ){
			//if (opType == OPARITH) {
			//	return INTEGER_SYM;
			//} else if (opType == OPREL) {
				return ft2;
			//} else {
			//	cout << "Error in Type checking" << endl;
			//	cout << "NUM and BOOL" << endl;
			//	cout << "Line NO. : " << tlist[i].lineno << endl;
			//	quitCompiler (315);
			
			//}

		} else {
			cout << "Error in Type checking" << endl;
			cout << "After Num" << endl;
			cout << "Line NO. : " << lineno << endl;
			quitCompiler (315);
		}
	} else if (t == STR){
		//cout << "ONE STR" << endl;
		tokenPt = i+1;
		// string
		// TBD
		outfile << "REG[" <<retREG <<"] = REG[1] ;//MEMPTR" << endl;
		//int memTmpPtr = memPtr;
		//outfile << "REG[" <<retREG <<"] = " << memTmpPtr <<" ;//MEMPTR" << endl;
		for (unsigned k = 1 ; k < varName.size() - 1 ; ++k ) {
			//outfile  << "MEM["<<memPtr<<"] = '"<< tlist[i].name[k] << "';" << endl;
			outfile  << "MEM[ REG [1] ] = \'"<< varName[k] << "\';" << endl;
			//++ memPtr;
			outfile << "REG[1] = REG[1] + 1; " << endl;
		}
		//outfile  << "MEM["<<memPtr<<"] = '\\0';" << endl;
		//++ memPtr;
		outfile << "MEM[ REG [1] ] = '\\0';" << endl;
		outfile << "REG[1] = REG[1] + 1; " << endl;
		return STRING_SYM;
	} else if (t == ID){
		//ft1 = symt.lookup(tlist[i].name, func_array[symt.func_cur].name);
		//cout << "lookup: " << tlist[i].name << ft1<< endl;
		// variable or function
		//parseNextToken();
		if (getTokenType(i+1) == LP ) {
			// add build in function support 

			ft1 = symt.lookupFunction(varName);
			if (ft1 == UNDEF_SYM){
				cout << "Function " << varName << " is not defined. " << endl;
				cout << "Error in parseExpression" << endl;
				quitCompiler (314);
			}
			tokenPt = i+2;
			//parseNextToken();
			//parseNextToken();
			// create a stack
			// clear the stack
			//list <int> argumentStack;
			int par_num = parseArgumentList( varName , 0);
			int func_call = symt.findFuncNodeNum(varName);
			outfile << "REG[0] = REG[0] - " << symt.func_array[func_call].stack_num - ( par_num + 2) << ";" << endl;
			//for (list<int>::const_iterator p =  argumentStack.begin(); p != argumentStack.end(); ++ p) {
			//	pushStack(*p);
			//}
			// push into stack. 
			//cout << "push return address" << endl;
			string addrLabel = "call_" + varName + "_end_";
			char buf[100];
			//itoa (retAddrPtr,buf,10);
			sprintf (buf, "%d", retAddrPtr);
			//sprintf_s (buf, "%d", retAddrPtr);
			addrLabel += buf;
			retAddrTable.push_back(addrLabel);
			int tmp = regPtr ++;
			outfile << "REG["<< tmp <<"] = "<< retAddrPtr << ";" << endl;
			++retAddrPtr; 
			pushStack(tmp);
			outfile << "goto " << varName <<" ;" << endl;
			outfile << addrLabel << ":" << endl;
			tmp = popStack();
			//cout << "pop out return value" << endl;
			//cout << "REG[retREG] = pop-out register" << endl;
			outfile << "REG["<< retREG<<"] = REG["<<tmp<<"] ;"<< endl;
			i=tokenPt;
			//parseNextToken();
			if (getTokenType(i) == RP ){
				tokenPt = i+1;
				//parseNextToken();
				ft2 = parseLeftExpression(tag, retREG);
			} else {
				// report Error
				cout << "Error in parseExpression" << endl;
				cout << "Expect right bracket after argument list" << endl;
				cout << "Name: " << getTokenName(i) << endl;
				cout << "Type: " << convertToken(getTokenType(i)) << endl;
				quitCompiler (314);
			}
			
		} else {
			int distance = 0;
			list<Symbol>::iterator p = symt.getSymbolPointer(varName, symt.func_array[symt.func_cur].name);
			// if the variable is coming from other function
			if ((p->func).compare( symt.func_array[symt.func_cur].name) != 0 ) {
				//ut << "Catch one" << endl;
				//it (2);
				int func_tmp = symt.func_cur;
				distance += symt.func_array[func_tmp].stack_num ;
				while ( (p->func).compare( symt.func_array[func_tmp].name) != 0  ) {
					func_tmp = symt.func_array[func_tmp].parent;
					distance += symt.func_array[func_tmp].stack_num ;
				}
				if (symt.func_array[func_tmp].parent == -1) {
					distance = 0;
				}
			} else {
				distance = symt.func_array[symt.func_cur].stack_num ;
			}
			//cout << "CHK:" << tlist[i].name <<" reg: "<< p->regPos << " mem: " << p->memPos << endl;
			//ft1 = symt.lookup(tlist[i].name, func_array[symt.func_cur].name);
			ft1 = p->type;
			//SymArray idarr = symt.isArray(tlist[i].name, func_array[symt.func_cur].name);
			if ( p->arr == UNDEF_ARRAY) {
				cout <<"Error: array type of variable:" << varName << "is not checked"  << endl;
				quitCompiler (315);
			} else if (p->arr == YES_ARRAY) {
				//cout << "var name: " << tlist[i].name << tlist[i+1].name << tlist[i+2].name<< endl;
				tokenPt = i+1;
			//	parseNextToken();
				expectKeyword(LSB,tokenPt,"array var", 315) ;
				//int index = parseArraySize();
				//index can be another expression
				// TBD
				int regIDX = regPtr ++;

				SymType ft3;
			   	ft3 = parseLongExpression(regIDX);
				expectKeyword(RSB,tokenPt,"array var", 315) ;
				if ((ft3 == INTEGER_SYM) ||( ft3 == BOOLEAN_SYM) ) {
				
				} else {
					cout << "Error in parseExpression" << endl;
					cout << "Expecting integer or boolean for index" << endl;
					quitCompiler (315);
				}

				//if (index > p->arrSize){
				//	cout << "index:" << index <<" is out of bound: " << p->arrSize << endl;
				//}
				if ( symt.func_array[symt.func_cur].parent == -1 || distance == 0) {
					int regTMP = regPtr ++;
					outfile << "REG["<<regTMP<<"] = REG["<<regIDX<<"] + "<<p->memPos<<" ; " << endl;
					outfile << "REG["<< retREG << "] = MEM[  REG["<< regTMP <<"] ] ;" << endl;
					//outfile << "REG["<< retREG << "] = MEM[ " << p->memPos <<" + REG["<< regIDX <<"] ] ;" << endl;
				} else {
					//outfile << "REG["<< retREG << "] = MEM[ REG[0] +" << symt.func_array[symt.func_cur].stack_num + 1 - p->memPos <<"- REG["<< regIDX <<"] ] ;" << endl;
					int regTMP = regPtr ++;
					outfile << "REG["<<regTMP<<"] = REG[0] + "<< distance + 1 - p->memPos << " ;" << endl;
					outfile << "REG["<<regTMP<<"] = REG["<<regTMP<<"] - REG["<<regIDX<<"] ;" << endl;
					outfile << "REG["<<retREG<<"] = MEM [ REG["<<regTMP<<"]] ;" << endl;
					//outfile << "REG["<< retREG << "] = MEM[ REG[0] +" << distance + 1 - p->memPos <<"- REG["<< regIDX <<"] ] ;" << endl;
				}
			} else {
				tokenPt = i+1;
				if ( symt.func_array[symt.func_cur].parent == -1 || distance == 0) {
					outfile << "REG["<< retREG << "] = MEM[ " << p->memPos <<"] ;" << endl;
				} else {
					int regTMP = regPtr ++;
					outfile << "REG["<< regTMP<<"] =  REG[0] + "<< distance + 1 - p->memPos<<";" << endl;
					outfile << "REG["<< retREG << "] = MEM[ REG[" << regTMP <<"]] ;" << endl;
					//outfile << "REG["<< retREG << "] = MEM[ REG[0] +" << symt.func_array[symt.func_cur].stack_num + 1 - p->memPos <<"] ;" << endl;
					//outfile << "REG["<< retREG << "] = MEM[ REG[0] +" << distance + 1 - p->memPos <<"] ;" << endl;
				}
			}

			// cout << "TESTME"<< tlist[tokenPt ].name << endl;
			
			ft2 = parseLeftExpression(tag , retREG);

//			cout << tlist[tokenPt -1 ].name << convertToken( tlist[tokenPt -1 ].type) << endl;
//			cout << tlist[tokenPt -2 ].name << convertToken( tlist[tokenPt -2 ].type) << endl;
//			cout << tlist[tokenPt -3 ].name << convertToken( tlist[tokenPt -3 ].type) << endl;
//			cout << tag << "lookup left: " << ft2 << endl;
		}

		if (ft2 == UNDEF_SYM || ft1 == ft2) {
			return ft1;
		} else if ( ft1 == INTEGER_SYM &&  ft2 == BOOLEAN_SYM){
			return ft2;
		} else {
			cout << "Error in Type checking" << endl;
			cout << "After Variable" << endl;
			cout << "Mismatch between " << ft1 << " And " << ft2 << endl;
			quitCompiler (315);
		}
	}
	return UNDEF_SYM;
}

int scanner::isBuildInFunction(string f){
	if (f.compare("getbool") == 0 || f.compare("getint") == 0 || f.compare("getstring") == 0) {
		return 1;
	} else if (f.compare("putbool") == 0 || f.compare("putint") == 0 || f.compare("putstring") == 0) {
		return 2;
	} else if (f.compare("sqrt") == 0 || f.compare("int2bool") == 0 || f.compare("bool2int") == 0) {
		return 3;
	} else {
		return 0;
	}
}


int scanner::parseArgumentList(string func, int parNum ){
	SymType ft1;
	SymType ft2;
	int regNum = regPtr++;
	ft1 = parseLongExpression(regNum);
	// NEED TBD some type checking here. 
	//cout << func << ":" << parNum << endl;
	ft2 = symt.lookupFunctionParam(func, parNum);
	if (ft2 == UNDEF_SYM ) {
		cout << "Error in line " << lineno << endl;
		cout << "Function " << func << " doesn't need the " << parNum+1 << "'s parameter." << endl;
		quitCompiler (315);
	}

    outfile << "MEM[REG[0]] = REG[ " <<regNum <<" ] ;"<<endl;
	outfile << "REG[0] = REG[0] - 1;"<<endl;

	//pushStack(regNum);
	//cout << "push function parameter: " << parNum << endl;
	//argumentStack->push_front(regNum);
	if (ft1 != ft2) {
		cout << "Error in typechecking in line: " << lineno << endl;
		cout << "function : parseArgumentList " << endl;
		cout << "mismatch between argument:" << converSymType(ft1) << " and function param: " << converSymType(ft2) << endl;
		quitCompiler(315);
	}


	if (tryKeyword (COMMAR, tokenPt,"parseArgumentList" )){
		return parseArgumentList(func, parNum+1 );
	//} else if (tryKeyword (RP, tokenPt,"parseArgumentList" )){
	//	return 0;
	//} else {
	//	cout << "Error in parseArgumentList" << endl;
	//	cout << "Expecting ID or FUNCTION" << endl;
	//	quitCompiler (315);
	}

	return parNum;
}

/*
int scanner::getStackValue(int index) {
	int regNum = regPtr++;
	outfile << "REG[ "<< regNum <<" ] = MEM[ REG[0] + " << index <<" ];"<<endl;
	return regNum;
}
*/
int scanner::pushStack ( int regNum){
//	outfile << endl;
	outfile << "MEM[REG[0]] = REG[ " <<regNum <<" ] ;"<<endl;
	outfile << "REG[0] = REG[0] - 1;"<<endl;
//	outfile << "REG[1] = REG[1] + 1;"<<endl;
	outfile << endl;
	return 0;
}

int scanner::popStack(){
	int regNum = regPtr++;
//	outfile << endl;
	outfile << "REG[0] = REG[0] + 1;"<<endl;
	outfile << "REG[ "<< regNum <<" ] = MEM[ REG[0] ];"<<endl;
	//outfile << "REG[1] = REG[1] + 1;"<<endl;
	outfile << endl;
	return regNum;
}

void scanner::GenWrapperFile (string fname) {
	ofstream wrapper_file(fname.c_str());

	wrapper_file << "#include <iostream> " << endl;
    wrapper_file << "#include \"stdio.h\" " << endl;
    wrapper_file << "#include \"stdlib.h\"" << endl;
    wrapper_file << "#include \"ctype.h\"" << endl;
    wrapper_file << "#include <cstring>" << endl;
    wrapper_file << "#include \"mem.h\"" << endl;
    wrapper_file << "#include \"func_build_in.h\"" << endl ;

    wrapper_file << "using namespace std;" << endl ;


    wrapper_file << "int main (){" << endl;
    wrapper_file << "    #include \"retaddr.cpp\"" << endl;
    wrapper_file << "    goto skip_buildin;" << endl;
    wrapper_file << "    #include \"build_in.cpp\"" << endl;
    wrapper_file << "skip_buildin:" << endl;
    wrapper_file << "    #include \"prog.cpp\"" << endl;
    wrapper_file << "    //cout << \"output is : \"<< REG[10] << endl; "<< endl;
    wrapper_file << "    return 0;" << endl;
    wrapper_file << "}" << endl;
    wrapper_file << "" << endl;
	wrapper_file.close();

}

void scanner::InitParamFile (){

outfile << "for (int i = 0; i<MAX_MEM; ++i) {" << endl;
outfile << "    MEM[i] = 1;" << endl;
outfile << "}" << endl;

outfile << "REG[0] = MAX_REG - 1;" << endl;
outfile << "REG[1] = MAX_MEM/4;" << endl;
outfile << "for (int i = 2; i<MAX_REG; ++i) {" << endl;
outfile << "    REG[i] = 0;" << endl;
outfile << "}" << endl;

}

void scanner::GenBuildInFuncFile (string fname){

	ofstream build_in_file(fname.c_str());

build_in_file << "" << endl ; 
build_in_file << "#define REG_TMP1 8" << endl ; 
build_in_file << "#define REG_TMP2 9" << endl ; 
build_in_file << "// getbool" << endl ; 
build_in_file << "goto getbool_end;" << endl ; 
build_in_file << "getbool :" << endl ; 
build_in_file << "REG[0] = REG[0] + 1;" << endl ; 
build_in_file << "REG[ REG_TMP1 ] = MEM [ REG[0] ] ;" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "REG [ REG_TMP2 ] = getbool();" << endl ; 
build_in_file << "MEM[ REG[0] ] = REG [ REG_TMP2 ] ;" << endl ; 
build_in_file << "REG[0] = REG[0] - 1;" << endl ; 
build_in_file << "goto *retAddr[ REG[REG_TMP1] ];" << endl ; 
build_in_file << "getbool_end:" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "// getint" << endl ; 
build_in_file << "goto getint_end;" << endl ; 
build_in_file << "getint :" << endl ; 
build_in_file << "REG[0] = REG[0] + 1;" << endl ; 
build_in_file << "REG[ REG_TMP1 ] = MEM [ REG[0] ] ;" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "REG[ REG_TMP2 ] = getint();" << endl ; 
build_in_file << "MEM[ REG[0] ] = REG [ REG_TMP2 ] ;" << endl ; 
build_in_file << "REG[0] = REG[0] - 1;" << endl ; 
build_in_file << "goto *retAddr[ REG[REG_TMP1] ];" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "getint_end:" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "// getstring" << endl ; 
build_in_file << "goto getstring_end;" << endl ; 
build_in_file << "getstring :" << endl ; 
build_in_file << "REG[0] = REG[0] + 1;" << endl ; 
build_in_file << "REG[ REG_TMP1 ] = MEM [ REG[0] ] ;" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "{" << endl ; 
build_in_file << "	string tmpSTR = getstring();" << endl ; 
build_in_file << "	//REG [ REG_TMP2 ] = memPtr ;" << endl ; 
build_in_file << "	REG [ REG_TMP2 ] = REG[1] ;" << endl ; 
build_in_file << "	for (unsigned int i = 0; i< tmpSTR.size(); ++i) {" << endl ; 
build_in_file << "		//MEM[ memPtr ++] = tmpSTR[i];" << endl ; 
build_in_file << "		MEM[ REG[1] ] = tmpSTR[i];" << endl ; 
build_in_file << "		REG[1] = REG[1] + 1;" << endl ; 
build_in_file << "	}" << endl ; 
build_in_file << "	//MEM[memPtr++] = '\\0';" << endl ; 
build_in_file << "	MEM[ REG[1] ] = '\\0';" << endl ; 
build_in_file << "	REG[1] = REG[1] + 1;" << endl ; 
build_in_file << "}" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "MEM[ REG[0] ] = REG [ REG_TMP2 ] ;" << endl ; 
build_in_file << "REG[0] = REG[0] - 1;" << endl ; 
build_in_file << "goto *retAddr[ REG[REG_TMP1] ];" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "getstring_end:" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "// putbool" << endl ; 
build_in_file << "goto putbool_end;" << endl ; 
build_in_file << "putbool :" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "REG[0] = REG[0] + 1;" << endl ; 
build_in_file << "REG[ REG_TMP1 ] = MEM [ REG[0] ] ;" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "REG[0] = REG[0] + 1;" << endl ; 
build_in_file << "REG[ REG_TMP2 ] = MEM [ REG[0] ] ;" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "putbool( REG[ REG_TMP2 ] );" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "REG [ REG_TMP2 ] = 1;" << endl ; 
build_in_file << "MEM[ REG[0] ] = REG [ REG_TMP2 ] ;" << endl ; 
build_in_file << "REG[0] = REG[0] - 1;" << endl ; 
build_in_file << "goto *retAddr[ REG[REG_TMP1] ];" << endl ; 
build_in_file << "putbool_end:" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "// putint" << endl ; 
build_in_file << "goto putint_end;" << endl ; 
build_in_file << "putint :" << endl ; 
build_in_file << "REG[0] = REG[0] + 1;" << endl ; 
build_in_file << "REG[ REG_TMP1 ] = MEM [ REG[0] ] ;" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "REG[0] = REG[0] + 1;" << endl ; 
build_in_file << "REG[ REG_TMP2 ] = MEM [ REG[0] ] ;" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "putint( REG[ REG_TMP2 ] );" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "REG[ REG_TMP2 ] = 1;" << endl ; 
build_in_file << "MEM[ REG[0] ] = REG [ REG_TMP2 ] ;" << endl ; 
build_in_file << "REG[0] = REG[0] - 1;" << endl ; 
build_in_file << "goto *retAddr[ REG[REG_TMP1] ];" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "putint_end:" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "// putstring " << endl ; 
build_in_file << "goto putstring_end;" << endl ; 
build_in_file << "putstring :" << endl ; 
build_in_file << "REG[0] = REG[0] + 1;" << endl ; 
build_in_file << "REG[ REG_TMP1 ] = MEM [ REG[0] ] ;" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "REG[0] = REG[0] + 1;" << endl ; 
build_in_file << "REG[ REG_TMP2 ] = MEM [ REG[0] ] ;" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "//putstring( itoa( REG[ REG_TMP2 ]) );" << endl ; 
build_in_file << "while (MEM[ REG[REG_TMP2] ] != '\\0') {" << endl ; 
build_in_file << "	cout << (char) MEM[ REG[REG_TMP2] ];" << endl ; 
build_in_file << "	REG[REG_TMP2] = REG[REG_TMP2]  + 1;" << endl ; 
build_in_file << "}" << endl ; 
build_in_file << "cout << endl;" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "REG[ REG_TMP2 ] = 1;" << endl ; 
build_in_file << "MEM[ REG[0] ] = REG [ REG_TMP2 ] ;" << endl ; 
build_in_file << "REG[0] = REG[0] - 1;" << endl ; 
build_in_file << "goto *retAddr[ REG[REG_TMP1] ];" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "putstring_end:" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "// sqrt " << endl ; 
build_in_file << "" << endl ; 
build_in_file << "goto sqrt_end;" << endl ; 
build_in_file << "sqrt :" << endl ; 
build_in_file << "REG[0] = REG[0] + 1;" << endl ; 
build_in_file << "REG[ REG_TMP1 ] = MEM [ REG[0] ] ;" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "REG[0] = REG[0] + 1;" << endl ; 
build_in_file << "REG[ REG_TMP2 ] = MEM [ REG[0] ] ;" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "REG[REG_TMP2] = sqrt (REG[ REG_TMP2 ] );" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "MEM[ REG[0] ] = REG [ REG_TMP2 ] ;" << endl ; 
build_in_file << "REG[0] = REG[0] - 1;" << endl ; 
build_in_file << "goto *retAddr[ REG[REG_TMP1] ];" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "sqrt_end:" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "// int2bool" << endl ; 
build_in_file << "goto int2bool_end;" << endl ; 
build_in_file << "int2bool :" << endl ; 
build_in_file << "REG[0] = REG[0] + 1;" << endl ; 
build_in_file << "REG[ REG_TMP1 ] = MEM [ REG[0] ] ;" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "REG[0] = REG[0] + 1;" << endl ; 
build_in_file << "REG[ REG_TMP2 ] = MEM [ REG[0] ] ;" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "REG[ REG_TMP2 ] = int2bool ( REG[ REG_TMP2 ] );" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "MEM[ REG[0] ] = REG [ REG_TMP2 ] ;" << endl ; 
build_in_file << "REG[0] = REG[0] - 1;" << endl ; 
build_in_file << "goto *retAddr[ REG[REG_TMP1] ];" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "int2bool_end:" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "// bool2int" << endl ; 
build_in_file << "goto bool2int_end;" << endl ; 
build_in_file << "bool2int :" << endl ; 
build_in_file << "REG[0] = REG[0] + 1;" << endl ; 
build_in_file << "REG[ REG_TMP1 ] = MEM [ REG[0] ] ;" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "REG[0] = REG[0] + 1;" << endl ; 
build_in_file << "REG[ REG_TMP2 ] = MEM [ REG[0] ] ;" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "REG[ REG_TMP2 ] = bool2int ( REG[ REG_TMP2 ] );" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "MEM[ REG[0] ] = REG [ REG_TMP2 ] ;" << endl ; 
build_in_file << "REG[0] = REG[0] - 1;" << endl ; 
build_in_file << "goto *retAddr[ REG[REG_TMP1] ];" << endl ; 
build_in_file << "" << endl ; 
build_in_file << "bool2int_end:" << endl ; 
build_in_file << "" << endl ; 

build_in_file.close();

}

void scanner::GenBuildInLibCPPFile (string fname){
	ofstream build_in_lib_cpp(fname.c_str());
build_in_lib_cpp << "" << endl ;
build_in_lib_cpp << "" << endl ;
build_in_lib_cpp << "#include \"func_build_in.h\"" << endl ;
build_in_lib_cpp << "" << endl ;
build_in_lib_cpp << "using namespace std;" << endl ;
build_in_lib_cpp << "" << endl ;
build_in_lib_cpp << "boolean getbool() {" << endl ;
build_in_lib_cpp << "	string bl;" << endl ;
build_in_lib_cpp << "	cin >> bl;" << endl ;
build_in_lib_cpp << "	if (bl.compare(\"true\") == 0) {" << endl ;
build_in_lib_cpp << "		return true;" << endl ;
build_in_lib_cpp << "	} else if (bl.compare(\"false\") == 0) {" << endl ;
build_in_lib_cpp << "		return false;" << endl ;
build_in_lib_cpp << "	} else {" << endl ;
build_in_lib_cpp << "		cout << \"Error in getbool! \" << bl << \"is not a boolean variable \" << endl;" << endl ;
build_in_lib_cpp << "		//exit (100);" << endl ;
build_in_lib_cpp << "		return false;" << endl ;
build_in_lib_cpp << "	}" << endl ;
build_in_lib_cpp << "}" << endl ;
build_in_lib_cpp << "" << endl ;
build_in_lib_cpp << "integer getint(){" << endl ;
build_in_lib_cpp << "	int i;" << endl ;
build_in_lib_cpp << "	cin >> i;" << endl ;
build_in_lib_cpp << "	return i;" << endl ;
build_in_lib_cpp << "}" << endl ;
build_in_lib_cpp << "" << endl ;
build_in_lib_cpp << "string getstring() {" << endl ;
build_in_lib_cpp << "	string str;" << endl ;
build_in_lib_cpp << "	cin >> str;" << endl ;
build_in_lib_cpp << "	return str;" << endl ;
build_in_lib_cpp << "}" << endl ;
build_in_lib_cpp << "" << endl ;
build_in_lib_cpp << "integer putbool(boolean bl) {" << endl ;
build_in_lib_cpp << "	if ( bl ) {" << endl ;
build_in_lib_cpp << "		cout << \"True\" << endl;" << endl ;
build_in_lib_cpp << "	} else {" << endl ;
build_in_lib_cpp << "		cout << \"False\" << endl;" << endl ;
build_in_lib_cpp << "	}" << endl ;
build_in_lib_cpp << "	return 0;" << endl ;
build_in_lib_cpp << "}" << endl ;
build_in_lib_cpp << "" << endl ;
build_in_lib_cpp << "integer putint(integer i) {" << endl ;
build_in_lib_cpp << "	cout << i << endl;" << endl ;
build_in_lib_cpp << "	return 0;" << endl ;
build_in_lib_cpp << "}" << endl ;
build_in_lib_cpp << "" << endl ;
build_in_lib_cpp << "integer putstring(string str) {" << endl ;
build_in_lib_cpp << "	cout << str << endl;" << endl ;
build_in_lib_cpp << "	return 0;" << endl ;
build_in_lib_cpp << "}" << endl ;
build_in_lib_cpp << "" << endl ;
build_in_lib_cpp << "integer sqrt(integer i) {" << endl ;
build_in_lib_cpp << "	return (int) sqrt ( (float ) i);  " << endl ;
build_in_lib_cpp << "}" << endl ;
build_in_lib_cpp << "" << endl ;
build_in_lib_cpp << "boolean int2bool(integer i){" << endl ;
build_in_lib_cpp << "	return (i == 0);" << endl ;
build_in_lib_cpp << "}" << endl ;
build_in_lib_cpp << "" << endl ;
build_in_lib_cpp << "integer bool2int(boolean bl){" << endl ;
build_in_lib_cpp << "	if (bl) {" << endl ;
build_in_lib_cpp << "		return 1;" << endl ;
build_in_lib_cpp << "	}" << endl ;
build_in_lib_cpp << "	return 0;" << endl ;
build_in_lib_cpp << "}" << endl ;
	build_in_lib_cpp.close();
}

void scanner::GenBuildInLibHFile (string fname){
	ofstream build_in_lib_h(fname.c_str());
build_in_lib_h << " " << endl ;
build_in_lib_h << "#ifndef _FUNC_BUILD_IN_H_" << endl ;
build_in_lib_h << "#define _FUNC_BUILD_IN_H_" << endl ;
build_in_lib_h << " " << endl ;
build_in_lib_h << "#include <iostream> " << endl ;
build_in_lib_h << "#include <string> " << endl ;
build_in_lib_h << "#include \"math.h\" " << endl ;
build_in_lib_h << " " << endl ;
build_in_lib_h << " " << endl ;
build_in_lib_h << "using namespace std; " << endl ;
build_in_lib_h << " " << endl ;
build_in_lib_h << "// build-in function " << endl ;
build_in_lib_h << "typedef bool boolean; " << endl ;
build_in_lib_h << "typedef int integer; " << endl ;
build_in_lib_h << " " << endl ;
build_in_lib_h << " " << endl ;
build_in_lib_h << "boolean getbool(); " << endl ;
build_in_lib_h << "integer getint(); " << endl ;
build_in_lib_h << "string getstring(); " << endl ;
build_in_lib_h << "integer putbool(boolean); " << endl ;
build_in_lib_h << "integer putint(integer); " << endl ;
build_in_lib_h << "integer putstring(string); " << endl ;
build_in_lib_h << "integer sqrt(integer); " << endl ;
build_in_lib_h << "boolean int2bool(integer); " << endl ;
build_in_lib_h << "integer bool2int(boolean); " << endl ;
build_in_lib_h << " " << endl ;
build_in_lib_h << "#endif " << endl ;
build_in_lib_h << " " << endl ;
	build_in_lib_h.close();
}


void  scanner::GenMemHFile (string fname){

	ofstream mem_h(fname.c_str());
	mem_h << "" << endl ;
	mem_h << "" << endl ;
	mem_h << "#ifndef _MEM_H_" << endl ;
	mem_h << "#define _MEM_H_" << endl ;
	mem_h << "" << endl ;
	mem_h << "#define MAX_MEM 33554432" << endl ;
	mem_h << "#define MAX_REG 10000" << endl ;
	mem_h << "" << endl ;
	mem_h << "" << endl ;
	mem_h << "int  MEM [MAX_MEM];" << endl ;
	mem_h << "int  REG [MAX_REG];" << endl ;
	mem_h << "" << endl ;
	mem_h << "" << endl ;
	mem_h << "// REG[0] always points to the empty block. " << endl ;
	mem_h << "" << endl ;
	mem_h << "#endif" << endl ;
	mem_h << "" << endl ;
	mem_h.close();

}


