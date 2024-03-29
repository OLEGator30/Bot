#ifndef COMMON_HPP
#define COMMON_HPP

#include "../poliz/poliz.hpp"

class PolizElem;

extern const char* StrKeyWords[10];

enum LexType { KeyWord, Function, Variable, Label, Number, String,
																	Equal, Operation, Bracket, Divider };

enum KeyWords { LexIf, LexGoto, LexPrint, LexInt, LexBuy, LexSell, LexProd,
																							LexJoin, LexBuild, LexTurn };

enum FuncName { Getid, Players, Bankbuy, Banksell, Buyprice, Sellprice,
											Money , Raw , Products , Factories , Isbdif ,
											Issdif , Getbdiff , Getsdiff };

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

struct PolizItem
{
	PolizElem *elem;
	PolizItem *next;

	PolizItem(PolizElem*);
};

struct labitem
{
	char *name;
	bool decl;
	PolizItem *val;
	labitem *next;

	labitem(char*);
	~labitem();
	void setval(PolizItem*);
};

struct varitem
{
	char *name;
	bool decl;
	int val;
	varitem *nextidx;
	varitem *next;

	varitem(char*);
	// varitem(const varitem&);
	~varitem();
};

class Tables
{
	labitem* lablist;
	varitem* varlist;

	public:

	Tables();
	~Tables();
	void addnewlab(labitem*);
	void addnewvar(varitem*);
	void decllab(char*,PolizItem*);
	void declvar(char*);
	labitem* foundlab(char*);
	varitem* foundvar(char*);
	void check();
};

extern Tables table;

int strtoint (char*);
const char* states2str(LexType);
void freemem(PolizItem*);
void printpoliz(PolizItem*);

#endif
