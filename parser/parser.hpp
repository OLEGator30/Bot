#ifndef PARSER_HPP
#define PARSER_HPP

#include "../common/common.hpp"
#include "../errors/errors.hpp"
#include "../scanner/scanner.hpp"
#include "../poliz/poliz.hpp"

class parser
{
	lexlist *curlex;
	PolizItem *poliz;
	PolizItem *curpolizelem;
	int line;

	void newlex();

	void Prog();
	void LabState();
	void Lab();
	void State();
	void Exp();
	void SimpleSt();
	bool VarArg();
	void Exp1();
	void Arg1();
	void Arg2();
	void Exp2();
	void Exp3();
	void IntFun();
	void Arg();
	void ArgList();
	void ArgPrint();

	bool Array(bool);
	int ComOp();
	int Op1();
	int Op2();

	void addpolizelem(PolizElem*);

	public:

	parser();
	PolizItem* run(lexlist*);
};

#endif
