
#include <string>
#include <ctype.h>
#include <iostream>
#include <vector>
#include <fstream>

#include "SymTable.h"
#include "compiler.h"


using namespace std;

#define MAX_PARSER_TOKEN_LENGTH 200

enum PSTATE {

	S0,S1,S2,S3,S4,S5,S6,S7,S8,S9,
	S10,S11,S12,S13,S14,S15,S16,S17,S18,S19,
	S20,S21,S22,S23,S24,S25,S26,S27,S28,S29,	
	SERROR
}; 
/*
struct funcNode {
	string name;
	int parent;
};
*/
class scanner {
private:
	ifstream infile;
	ofstream outfile;
	ofstream retfile;
	int lineno;
	vector <char> parStack;
	PSTATE st;

	token tcur;
	vector<token> tlist;
	vector<string> lineinfo;
//	vector<funcNode> func_array;
//	int func_cur;
//	string line;
	
	// parser
	unsigned int tokenPt;

	// symbol table
	SymTable symt;

	// regPtr
	int regPtr;

	// memPtr;
	int memPtr;
	int stackPtr ; 

	// label_ptr
	int labelIfPtr ;
	int labelWhilePtr ;
	int labelCasePtr ;
	int retAddrPtr ;

	// ret addr
	// ret register for function
	//map < string , int > retAddr;
	// ret addr table
	vector <string> retAddrTable;


public:
	scanner(void){
		st = S0;
		lineno = 1;
		tokenPt =0;
		//func_cur = -1;
		regPtr = 10;
		memPtr = 0;
		stackPtr = 5000;
		labelIfPtr =0;
		labelWhilePtr = 0;
		labelCasePtr = 0;
		retAddrPtr = 0;
	};

	~scanner(){};
private:
	void quitCompiler (int ret) ;
	PSTATE get_next_state( DETAIL_TOKEN_TYPE t);

	DETAIL_TOKEN_TYPE is_keyword (string word);
	DETAIL_TOKEN_TYPE get_bracket_type (char c);
	bool is_oprand_arith(char c);
	bool is_oprand_term(char c);
	bool is_oprand_e1_relation(char c);
	bool is_oprand_e2_relation(char c);
	bool is_oprand(char c);
	bool is_bracket(char c);
	bool is_valid_id_char(char c);
	bool is_continued_id_char(char c);
	bool is_valid_str_char(char c);
	int is_accepted_char(char c);
public:
	int readFile (string fname);
	int openFile (string fname);
	int closeFile ();
private:
	int getToken (string line);
	char ParseNextChar (int opt = 0) ;
	void PutBackNextChar (char c) ;
	int parseNextToken ();
	string getTokenName(unsigned int i);
	DETAIL_TOKEN_TYPE getTokenType(unsigned int i);
	int insertToken(DETAIL_TOKEN_TYPE t, string name);
	string convertToken(DETAIL_TOKEN_TYPE t);
	string converSymType( SymType ft);
	int parCheck(char c);
public:
	void listToken(void){
		unsigned int i;
		for(i= 0; i < tlist.size(); i++){
			cout << i <<"|"<< tlist[i].type <<"|"<< tlist[i].name << endl;
		}
	};

private:
	//void insertFunctionName (string name);
	int getParentFunction( void );


	// recursive parser
public:
	int parseStart(void);
private:
	int parseFunction (void) ;
	int parseFunctionHeader(void);
	int parseFunctionBody(void);
	int parseParameterList( int index );
	int parseParameter(int index);
	int parseArraySize(void);
	int parseLineDeclaration( void );
	int parseFuncBodyDeclaration( void );
	int parseLineStatement(void);
	int parseStatement(void );
	SymType parseAssignStatement(int regNum);
	int parseIfStatement(void);
	int parseWhileStatement(void);
	int parseCaseStatement(void);
	SymType parseLeftExpression(int tag, int retREG);
	SymType parseExpression(int tag, int retREG);
	SymType parseLongExpression( int retREG);
	int parseArgumentList(string func, int i);
	int parseRelationOprand(void);
	bool expectKeyword(DETAIL_TOKEN_TYPE t,unsigned int i,string function,int exitVal);
	bool tryKeyword(DETAIL_TOKEN_TYPE t,unsigned int i,string function);

	int getStackValue(int index) ;
	int pushStack (int regNum);
	int popStack();
	int genReturnAddrTable();

	int isBuildInFunction(string f);
	SymType getBuildInFuncType(string f);
	
	void GenWrapperFile (string fname);
	void InitParamFile();
	void GenBuildInFuncFile (string fname);
	void GenBuildInLibCPPFile (string fname);
	void GenBuildInLibHFile (string fname);
	void GenMemHFile (string fname);

private:
//		int regCnt;
	string fname;
};

