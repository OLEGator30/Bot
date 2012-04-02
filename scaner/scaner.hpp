#ifndef SCANER_HPP
#define SCANER_HPP

#include "../buffer/buffer.hpp"

extern const char* StrKeyWords[9];

enum FSMState { Home, Num, Fun, Var, Lab, Key, Str, Equ };
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
	FSMState state;
	classbuf buffer;

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
	lexlist* addnewlex(char*,LexType);

	public:

	fsm(): state(Home) {}
	lexlist* newchar(char);
};

class scaner
{
	fsm myfsm;
	int line;
	lexlist* begin;
	lexlist* end;

	lexlist* addnewlex(lexlist*,lexlist*,int);

	public:

	scaner(): line(1), begin(0), end(0) {};
	lexlist* run(int);
};

#endif
