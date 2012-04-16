#ifndef SCANER_HPP
#define SCANER_HPP

#include "../common/common.hpp"
#include "../buffer/buffer.hpp"
#include "../errors/errors.hpp"

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
