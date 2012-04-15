#ifndef SCANER_HPP
#define SCANER_HPP

#include "../buffer/buffer.hpp"
#include "../errors/errors.hpp"

extern const char* StrKeyWords[9];

enum LexType { KeyWord, Function, Variable, Label, Number, String,
																	Equal, Operation, Bracket, Divider };

int strtoint (char*);
const char* states2str(LexType);

struct lexlist
{
	char* lexstr;
	int lexnum;
	LexType type;
	int line;
	lexlist* next;

	lexlist();
	void print();
};

class fsm
{
	enum FSMState { Home, Num, Fun, Var, Lab, Key, Str, Equ } state;
	classbuf buffer;
	int line;

	bool inbrackets(char);
	bool individers(char);
	bool inspaces(char);
	bool inchar(char);
	bool inoperations(char);
	lexlist* homeproc(char);
	lexlist* numproc(char);
	lexlist* funproc(char);
	lexlist* varproc(char);
	lexlist* labproc(char);
	lexlist* keyproc(char);
	lexlist* equproc(char);
	lexlist* addnewlex(char,LexType);
	lexlist* addnewlex(LexType);
	lexlist* addnewlex();

	public:

	fsm(): state(Home) {}
	lexlist* newchar(int,int);
};

class scaner
{
	fsm myfsm;
	int line;
	lexlist* begin;
	lexlist* end;

	lexlist* addnewlex(lexlist*,lexlist*);

	public:

	scaner(): line(1), begin(0), end(0) {};
	lexlist* run(int);
};

#endif
