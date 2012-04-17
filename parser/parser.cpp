#include "parser.hpp"

void parser::newlex()
{
	curlex=curlex->next;
}

void parser::run(lexlist *l)
{
	curlex=l;
	Prog();
}

void parser::Prog()
{
	Block();
	if (curlex->type!=KeyWord||curlex->lexnum!=sizeof(StrKeyWords))
		throw parserr("expected `EOF'",curlex);
}

void parser::Block()
{
	if (curlex->type==Bracket&&curlex->lexnum=='{')
	{
		newlex();
		do
		{
			LabState();
		} while (curlex->type!=Bracket||curlex->lexnum!='}');
		newlex();
	}
	else throw parserr("expexted `{'",curlex);
}

void parser::LabState()
{
	Lab();
	State();
}

void parser::Lab()
{
	if (curlex->type==Label)
	{
		// label declaraiton
		newlex();
		if (curlex->type==Divider&&curlex->lexnum==':')
			newlex();
		else throw parserr("expected `:'\n",curlex);
	}
}

void parser::State()
{
	if ((curlex->type==KeyWord)&&(curlex->lexnum==LexIf))
	{
		newlex();
		if (curlex->type==Bracket&&curlex->lexnum=='(')
		{
			newlex();
			Exp();
			if (curlex->type==Bracket&&curlex->lexnum==')')
			{
				newlex();
				SimpleSt();
			}
			else throw parserr("expected `)'",curlex);
		}
		else throw parserr("expected `('",curlex);
	}
	else
	{
		SimpleSt();
		if (curlex->type==Divider&&curlex->lexnum==';')
			newlex();
		else throw parserr("expected `;'",curlex);
	}
}

void parser::SimpleSt()
{
	if (curlex->type==KeyWord&&curlex->lexnum==LexInt)
	{
		do
		{
			newlex();
			if (curlex->type==Variable)
			{
				int k;
				
				newlex();
				k=Array();
				if (k)
				{
					// array declaration. size=k
				}
				else
				{
					// variable declaration
					VarArg();
				}
			}
			else throw parserr("declaration of variable expected",curlex);
		} while (curlex->type==Divider&&curlex->lexnum==',');
	}
	else if (curlex->type==Variable)
	{
		int k;

		newlex();
		k=Array();
		if (curlex->type==Operation&&curlex->lexnum=='=')
		{
			newlex();
			Exp1();
		}
		else parserr("expected `='",curlex);
	}
	else if (curlex->type==KeyWord&&curlex->lexnum==LexGoto)
	{
		newlex();
		if (curlex->type==Label)
			newlex();
		else parserr("name of label expected",curlex);
	}
	else if (curlex->type==KeyWord&&curlex->lexnum==LexPrint)
	{
		newlex();
		ArgList();
	}
	else if (curlex->type==KeyWord&&curlex->lexnum==LexSell)
	{
		newlex();
		Arg2();
	}
	else if (curlex->type==KeyWord&&curlex->lexnum==LexBuy)
	{
		newlex();
		Arg2();
	}
	else if (curlex->type==KeyWord&&curlex->lexnum==LexProd)
	{
		newlex();
		Arg1();
	}
	else if (curlex->type==KeyWord&&curlex->lexnum==LexBuild)
		newlex();
	else if (curlex->type==KeyWord&&curlex->lexnum==LexEndturn)
		newlex();
	else throw parserr("expected ...",curlex);
}

void parser::Exp()
{
	Exp1();
	if (ComOp())
		Exp1();
}

void parser::Exp1()
{
	Exp2();
	while (Op1())
		Exp2();
}

void parser::Exp2()
{
	Exp3();
	while (Op2())
		Exp2();
}

void parser::Exp3()
{
	if (curlex->type==Variable)
	{
		newlex();
	}
	else if (curlex->type==Number)
	{
		newlex();
	}
	else if (curlex->type==Function)
	{
		newlex(); // ??
		newlex(); // ??
		newlex(); // ??
	}
	else if (curlex->type==Operation&&curlex->lexnum=='-')
	{
		newlex();
		Exp3();
	}
	else if (curlex->type==Operation&&curlex->lexnum=='!')
	{
		newlex();
		Exp3();
	}
	else if (curlex->type==Bracket&&curlex->lexnum=='(')
	{
		newlex();
		Exp();
		if (curlex->type==Bracket&&curlex->lexnum==')')
			newlex();
		else throw parserr("expected `)'",curlex);
	}
	else throw parserr("expected ...",curlex);
}

bool parser::ComOp()
{
	if (curlex->type==Equal)
	{
		newlex();
		return true;
	}
	else if (curlex->type==Operation&&curlex->lexnum=='<')
	{
		newlex();
		return true;
	}
	else if (curlex->type==Operation&&curlex->lexnum=='>')
	{
		newlex();
		return true;
	}
	return false;
}

bool parser::Op1()
{
	if (curlex->type==Operation&&curlex->lexnum=='+')
	{
		newlex();
		return true;
	}
	if (curlex->type==Operation&&curlex->lexnum=='-')
	{
		newlex();
		return true;
	}
	if (curlex->type==Operation&&curlex->lexnum=='|')
	{
		newlex();
		return true;
	}
	return false;
}

bool parser::Op2()
{
	if (curlex->type==Operation&&curlex->lexnum=='*')
	{
		newlex();
		return true;
	}
	if (curlex->type==Operation&&curlex->lexnum=='/')
	{
		newlex();
		return true;
	}
	if (curlex->type==Operation&&curlex->lexnum=='%')
	{
		newlex();
		return true;
	}
	if (curlex->type==Operation&&curlex->lexnum=='&')
	{
		newlex();
		return true;
	}
	return false;
}

void parser::Arg1()
{
	if (curlex->type==Bracket&&curlex->lexnum=='(')
	{
		newlex();
		Exp1();
		if (curlex->type==Bracket&&curlex->lexnum==')')
			newlex();
		else throw parserr("expected `)'",curlex);
	}
	else throw parserr("expected `('",curlex);
}

void parser::Arg2()
{
	if (curlex->type==Bracket&&curlex->lexnum=='(')
	{
		newlex();
		Exp1();
		if (curlex->type==Divider&&curlex->lexnum==',')
		{
			newlex();
			Exp1();
			if (curlex->type==Bracket&&curlex->lexnum==')')
				newlex();
			else throw parserr("expected `)'",curlex);
		}
		else throw parserr("expected `,'",curlex);
	}
	else throw parserr("expected `('",curlex);
}

void parser::VarArg()
{
	if (curlex->type==Operation&&curlex->lexnum=='=')
	{
		newlex();
		Exp1();
	}
}

int parser::Array()
{
	if (curlex->type==Bracket&&curlex->lexnum=='[')
	{
		newlex();
		Exp1();
		if (curlex->type==Bracket&&curlex->lexnum==']')
			newlex();
		else throw parserr("expected `]'",curlex);
	}
	return 0;
}

void parser::ArgList()
{
	ArgPrint();
	while (curlex->type==Divider&&curlex->lexnum==',')
	{
		newlex();
		ArgPrint();
	}
}

void parser::ArgPrint()
{
	if (curlex->type==String)
		newlex();
	else
		Exp1();
}
