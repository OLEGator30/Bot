#ifndef PARSER_HPP
#define PARSER_HPP

#include "../common/common.hpp"
#include "../errors/errors.hpp"
#include "../scanner/scanner.hpp"

class parser
{
	lexlist *curlex;

	void newlex();

	void Prog();
	void Block();
	void LabState();
	void Lab();
	void State();
	void Exp();
	void SimpleSt();
	void VarArg();
	void Exp1();
	void Arg1();
	void Arg2();
	void Exp2();
	void Exp3();
	void IntFun();
	void Arg();
	void ArgList();
	void ArgPrint();

	int Array();

	bool ComOp();
	bool Op1();
	bool Op2();

	public:

	parser(){}
	void run(lexlist*);
};

#endif
