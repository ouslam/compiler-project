


/*
enum TOKEN_TYPE {
	// initial
	UNDEF0,
	// integer, boolean, string
	MARKER,
	// function, begin, end, if, then, else, while, case, is, when, dfault, and , or, not
	KEYWORD,
	// [ ] ( ) + - & | < > == != := * /
	SPECIALCHAR,
	// [0-9]+
	NUM,
	// "[a-zA-Z0-9]*"
	STR,
	// [a-zA-Z][a-zA-Z0-9_]*
	ID,
	// , 
	COMMAR,
	// ;
	SEMI
};
*/
enum DETAIL_TOKEN_TYPE{
	// initial
	UNDEF,
	// integer, boolean, string
	MARKER,

	// function, begin, end, if, then, else, while, case, is, when, default, and , or, not
	FUNCTION,
	// KEYWORD,
	BEGIN,END,IF,THEN,ELSE,WHILE,CASE,IS,WHEN,DEFAULT,AND,OR,NOT,

	// [0-9]+
	NUM,
	// "[a-zA-Z0-9]*"
	STR,
	// [a-zA-Z][a-zA-Z0-9_]*
	ID,
	// , 
	COMMAR,
	// ;
	SEMI,


	//OPRAND_ARITH
	OPARITH,
	/*
	// +
	ADD,
	// -
	MINUS,
	// &
	AND,
	// |
	OR,
	*/

	// OPRAND_REL
	OPREL,
	/*
	// <
	LT,
	// >
	GT,
	// == 
	EQ,
	// !=
	NE,
	*/

	// OPRAND_TERM 
	OPTERM,
	/*
	// *
	MUL,
	// /
	DIV,
	*/

	//BRACKET 
	// ( )
	LP,RP,
	// [ ]
	LSB, RSB,
	// { }
	LB, RB,

	// :=
	ASSIGN

};


struct token{
	DETAIL_TOKEN_TYPE type;
	int lineno;
	string name;
	int level;
};


