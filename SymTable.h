
#include <string>
#include <ctype.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <list>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

enum SymType {UNDEF_SYM, INTEGER_SYM, STRING_SYM, BOOLEAN_SYM};
enum SymClass {UNDEF_CLASS, FUNCTION_CLASS, VAR_CLASS, PAR_CLASS};
enum SymArray {UNDEF_ARRAY, YES_ARRAY, NO_ARRAY};



class Symbol  {
public:
	string func;
	SymType type;
	SymClass cls;
	SymArray arr;
	int arrSize;
	int memPos;
	int regPos;
	Symbol(){};
	~Symbol(){};
};

class funcNode {
public:
    string name;
	int stack_num;
    int parent;
	funcNode(){
		stack_num = 1 ;
		parent = -1;
	};
	~funcNode(){};
};


class SymTable{
public:
	list<Symbol> EMPTY_PTR;
	int func_cur;
private:
	map <string,list<Symbol> > sym_table;
	set <string> sym_index_set;

public:
	map <string , list<Symbol> > func_par_table;
	set <string> func_index_set;

	vector<funcNode> func_array;

public:
	SymTable (){
		func_cur = -1;
		InsertBuildInFunction();
	};
	~SymTable(){};
	void InsertBuildInFunction(void);
	int insertFunctionName (string name);
	int getParentFunction( void );
	SymType convert(string s){
		if (s.compare("string") == 0 ){
			return STRING_SYM;
		} else if (s.compare("integer") == 0 ){
			return INTEGER_SYM;
		} else if (s.compare("boolean") == 0 ){
			return BOOLEAN_SYM;
		}  else {
			// report error
			return UNDEF_SYM;
		}
	};
//	int insert(string func, string name, SymType type, SymClass cls, SymArray arr, int arrSize);
//	int insert(string func, string name, SymType type, SymClass cls, SymArray arr);
//	int insert(string func, string name, SymType type, SymClass cls);
//	int insert(string func, string name, SymType type);
	list<Symbol>::iterator insert(string func, string name);
	int insertFuncPar(string func, string name, SymType type, SymClass cls, SymArray arr);

	SymType lookupFunction (string name);
//	SymType lookup (string name, string function);
	SymType lookupFunctionParam (string func, int i);
//	SymArray isArray(string name, string function);
//	int getArraySize(string name, string function);
//	void updateArrayType (string name, string function, SymArray arr);
//	void updateArraySize (string name, string function, int arrSize);
//	int getMemPosition(string name, string function);
//	int getRegPosition(string name, string function);
	int findFuncNodeNum (string func) ;
	list<Symbol>::iterator getSymbolPointer (string name, string function);
private:
};
