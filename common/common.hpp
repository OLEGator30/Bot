#ifndef COMMON_HPP
#define COMMON_HPP

extern const char* StrKeyWords[9];

enum LexType { KeyWord, Function, Variable, Label, Number, String,
																	Equal, Operation, Bracket, Divider };

enum KeyWords { LexIf, LexGoto, LexPrint, LexInt, LexBuy, LexSell, LexProd,
																								LexBuild, LexEndturn };

struct lexlist
{
	char* lexstr;
	int lexnum, line;
	LexType type;
	lexlist* next;

	lexlist();
	~lexlist() {}
	void print();
};

struct labitem
{
	char *name;
	bool decl;
	int val;
	labitem *next;

	labitem(char*);
	~labitem();
};

struct varitem
{
	char *name;
	bool decl;
	int val;
	// int idx;
	// varitem *nextidx;
	varitem *next;

	varitem(char*);
	~varitem();
};

class Tables
{
	labitem* lablist;
	varitem* varlist;
	labitem* foundlab(char*);
	varitem* foundvar(char*);

	public:

	Tables();
	void addnewlab(labitem*);
	void addnewvar(varitem*);
	void decllab(char*,int);
	void declvar(char*);
	varitem* getvar(char*);
	bool check();
};

extern Tables table;

int strtoint (char*);
const char* states2str(LexType);

#endif
