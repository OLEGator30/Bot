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
	void ComOp();
	void Exp2();
	void Exp3();
	void Op1();
	void Op2();
	void IntFun();
	void Arg();
	void ArgList();
	void ArgPrint();

	public:

	parser(){}
	void run(lexlist*);
};

#endif
