

#include "SymTable.h"

list<Symbol>::iterator SymTable::insert(string func, string name){
	Symbol s;
	s.func = func;
	s.type = UNDEF_SYM;
	s.cls = UNDEF_CLASS;
	s.arr = UNDEF_ARRAY;
	s.arrSize = 0;
	sym_table[name].push_back(s);
	sym_index_set.insert(name);
	list<Symbol>::iterator p = sym_table[name].end();
	--p;

//	cout << "testme" << endl;
//	cout << p->func << endl;
//	cout << "testme2" << endl;
	return p;
}
/*
int SymTable::insert(string func, string name, SymType type, SymClass cls, SymArray arr){
	Symbol s;
	s.func = func;
	s.type = type;
	s.cls = cls;
	s.arr = arr;
	sym_table[name].push_back(s);
	sym_index_set.insert(name);
	return 0;
}
*/

int SymTable::insertFuncPar(string func, string name, SymType type, SymClass cls, SymArray arr){
	Symbol s;
	s.func = func;
	s.type = type;
	s.cls  = cls;
	s.arr = arr;
	func_par_table[func].push_back(s);
	func_index_set.insert(func);
	return 0;
}

/*
int SymTable::insert(string func, string name, SymType type){
	return insert (func, name, type, UNDEF_CLASS, UNDEF_ARRAY);
}

*/
/*
SymType SymTable::lookup (string name, string function){
//	bool found = false;
	if (sym_index_set.find (name) == sym_index_set.end()) {
		return UNDEF_SYM;
	} else {
		
		for (list<Symbol>::const_iterator p1 = sym_table[name].begin(); p1!= sym_table[name].end() ; ++p1) {
			if (p1->func == function) {
				return p1->type;
			}
		}
		return UNDEF_SYM;
	}
}
*/
SymType SymTable::lookupFunction (string name){
//	bool found = false;
	if (sym_index_set.find (name) == sym_index_set.end()) {
		return UNDEF_SYM;
	} else {
		return sym_table[name].front().type;
	}

}

SymType SymTable::lookupFunctionParam (string func, int i){
	int k = 0;
	if (func_index_set.find(func) == func_index_set.end()) {
		//if ( func_index_set.back().compare(func) != 0 ) {
		//for (set<string>::iterator p = func_index_set.begin() ; p!= func_index_set.end() ; ++ p) {
		//	cout << *p << endl;
		//}
		//cout << "Func: " << func << endl;
			return UNDEF_SYM;
		//}
	}

	list<Symbol>::const_iterator p = func_par_table[func].begin();
	while (k<i) {
		++p;
		if (p == func_par_table[func].end()){
			cout << "out of bound in " << func << " " << i << "'s param" << endl;
			exit (315);
		}
		++k;
	}
	return p->type;
}

/*
SymArray SymTable::isArray(string name, string function){
//	bool found = false;
	if (sym_index_set.find (name) == sym_index_set.end()) {
		return UNDEF_ARRAY;
	} else {
		
		for (list<Symbol>::const_iterator p1 = sym_table[name].begin(); p1!= sym_table[name].end() ; ++p1) {
			if (p1->func == function) {
				return p1->arr;
			}
		}
		return UNDEF_ARRAY;
	}

}
*/
int SymTable::findFuncNodeNum (string func) {
	int i = func_array.size() - 1;
	while ( i >= 0 ) {
		if (func_array[i].name.compare(func) == 0) {
			return i;
		}
		--i;
	}
	return -1;
}

list<Symbol>::iterator SymTable::getSymbolPointer (string name, string function){
//	bool found = false;
	if (sym_index_set.find (name) == sym_index_set.end()) {
		return EMPTY_PTR.begin();
	} else {
		for (list<Symbol>::iterator p1 = sym_table[name].begin(); p1!= sym_table[name].end() ; ++p1) {
			if ( (p1->func).compare( function) == 0) {
				return p1;
			}
		}
		if (! sym_table[name].empty()) {
			// a lot of trouble to trace upward. 
			int i = findFuncNodeNum (function);
			if (i < 0) {
				return EMPTY_PTR.begin();
			}
			i = func_array[i].parent;
			while (i >= 0) {
				for (list<Symbol>::iterator p1 = sym_table[name].begin(); p1!= sym_table[name].end() ; ++p1) {
					if ((p1->func).compare (func_array[i].name) == 0) {
						return p1;
					}
				}
				//cout << " find parent" << endl;
				i = func_array[i].parent;
			}
			return EMPTY_PTR.begin();
			//return  sym_table[name].begin();
		}
		cout << "SymTable: Potential Error" << endl;
		return EMPTY_PTR.begin();
	}


}

int SymTable::getParentFunction( void ){
    func_cur = func_array[func_cur].parent;
    return func_cur;
}


int SymTable::insertFunctionName (string name){
	if (func_cur == -1) {
		funcNode fn;
		fn.name = name;
		fn.parent = -1;
		func_array.push_back(fn);
		func_cur = 0;
	} else {
		vector<funcNode>::iterator pos;
		for (pos = func_array.begin(); pos != func_array.end(); ++ pos) {
			if ( pos->name.compare(name) == 0){
				return -1;
				//cout << "Function name conflicts in line: " << lineno << endl;
				//cout << "Function " << name << " is already defined" << endl;
				//quitCompiler (315);
			}
		}
		
		funcNode fn;
		fn.name = name;
		fn.parent = func_cur;
		func_array.push_back(fn);
		func_cur = func_array.size() - 1;
//		cout << "CUR: "<<func_cur << endl;
	}
	return func_cur;
}

void SymTable::InsertBuildInFunction(void){
	list<Symbol>::iterator p ;
	funcNode fn[9];
	string f;
	f = "getbool";
	p = insert("none",f);
	p->type = BOOLEAN_SYM;
	fn[0].name = f;
	fn[0].stack_num = 1;
	func_array.push_back(fn[0]);
	f = "getint";
	p = insert("none",f);
	p->type = INTEGER_SYM;
	fn[1].name = f;
	fn[1].stack_num = 1;
	func_array.push_back(fn[1]);
	f = "getstring";
	p = insert("none",f);
	p->type = STRING_SYM;
	fn[2].name = f;
	fn[2].stack_num = 1;
	func_array.push_back(fn[2]);

	f = "putbool";
	p = insert("none",f);
	p->type = INTEGER_SYM;
	insertFuncPar (f,"par", BOOLEAN_SYM , PAR_CLASS, NO_ARRAY);
	fn[3].name = f;
	fn[3].stack_num = 2;
	func_array.push_back(fn[3]);
	f = "putint";
	p = insert("none",f);
	p->type = INTEGER_SYM;
	insertFuncPar (f,"par", INTEGER_SYM , PAR_CLASS, NO_ARRAY);
	fn[4].name = f;
	fn[4].stack_num = 2;
	func_array.push_back(fn[4]);
	f = "putstring";
	p = insert("none",f);
	p->type = INTEGER_SYM;
	insertFuncPar (f,"par", STRING_SYM , PAR_CLASS, NO_ARRAY);
	fn[5].name = f;
	fn[5].stack_num = 2;
	func_array.push_back(fn[5]);

	f = "sqrt";
	p = insert("none",f);
	p->type = INTEGER_SYM;
	insertFuncPar (f,"par", INTEGER_SYM , PAR_CLASS, NO_ARRAY);
	fn[6].name = f;
	fn[6].stack_num = 2;
	func_array.push_back(fn[6]);
	f = "int2bool";
	p = insert("none",f);
	p->type = BOOLEAN_SYM;
	insertFuncPar (f,"par", INTEGER_SYM , PAR_CLASS, NO_ARRAY);
	fn[7].name = f;
	fn[7].stack_num = 2;
	func_array.push_back(fn[7]);
	f = "bool2int";
	p = insert("none",f);
	p->type = INTEGER_SYM;
	insertFuncPar (f,"par", BOOLEAN_SYM , PAR_CLASS, NO_ARRAY);
	fn[8].name = f;
	fn[8].stack_num = 2;
	func_array.push_back(fn[8]);

}

