#include "../functions/functions.hpp"
#include "../errors/errors.hpp"
#include "parser.hpp"

parser::parser(): poliz(0), curpolizelem(0) {}

void parser::newlex()
{
	lexlist *temp=curlex;
	curlex=curlex->next;
	line=curlex->line;
	delete temp;
}

PolizItem* parser::run(lexlist *l)
{
	curlex=l;
	line=curlex->line;
	Prog();
	return poliz;
}

void parser::Prog()
{
	while (curlex->type!=KeyWord||
											curlex->lexnum!=sizeof(StrKeyWords)/sizeof(void*))
		LabState();
	delete curlex;
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
		table.decllab(curlex->lexstr,curpolizelem);
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
				PolizLabel *temp=new PolizLabel((char*)0);
				addpolizelem(temp);
				PolizElem *temp1=new PolizOpGoFalse;
				addpolizelem(temp1);
				newlex();
				LabState();
				temp->SetVal(curpolizelem);
			}
			else throw parserr("expected `)'",curlex);
		}
		else throw parserr("expected `('",curlex);
	}
	else if (curlex->type==Bracket&&curlex->lexnum=='{')
	{
		newlex();
		do
		{
			LabState();
		} while (curlex->type!=Bracket||curlex->lexnum!='}');
		newlex();
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
				PolizItem *tmpcur=curpolizelem;
				table.declvar(curlex->lexstr);
				PolizElem *temp=new PolizVarAddr(curlex->lexstr,0);
				addpolizelem(temp);
				newlex();
				if (!Array(true))
					if (!VarArg())
					{
						delete temp;
						curpolizelem=tmpcur;
						if (tmpcur)
							tmpcur->next=0;
						else
							poliz=0;
					}
			}
			else
				throw parserr("declaration of variable or array expected",curlex);
		} while (curlex->type==Divider&&curlex->lexnum==',');
	}
	else if (curlex->type==Variable)
	{
		PolizElem *temp=new PolizVarAddr(curlex->lexstr,0);
		addpolizelem(temp);
		newlex();
		Array(false);
		if (curlex->type==Operation&&curlex->lexnum=='=')
		{
			newlex();
			Exp1();
			temp=new PolizFunAssig;
			addpolizelem(temp);
		}
		else parserr("expected `='",curlex);
	}
	else if (curlex->type==KeyWord&&curlex->lexnum==LexGoto)
	{
		newlex();
		if (curlex->type==Label)
		{
			PolizElem *temp=new PolizLabel(curlex->lexstr);
			addpolizelem(temp);
			temp=new PolizOpGo;
			addpolizelem(temp);
			newlex();
		}
		else parserr("name of label expected",curlex);
	}
	else if (curlex->type==KeyWord&&curlex->lexnum==LexPrint)
	{
		PolizElem *temp=new PolizPrintEnd;
		addpolizelem(temp);
		newlex();
		ArgList();
		temp=new PolizPrint;
		addpolizelem(temp);
	}
	else if (curlex->type==KeyWord&&curlex->lexnum==LexSell)
	{
		newlex();
		Arg2();
		PolizElem *temp=new PolizSell;
		addpolizelem(temp);
	}
	else if (curlex->type==KeyWord&&curlex->lexnum==LexBuy)
	{
		newlex();
		Arg2();
		PolizElem *temp=new PolizBuy;
		addpolizelem(temp);
	}
	else if (curlex->type==KeyWord&&curlex->lexnum==LexProd)
	{
		newlex();
		Arg1();
		PolizElem *temp=new PolizProd;
		addpolizelem(temp);
	}
	else if (curlex->type==KeyWord&&curlex->lexnum==LexJoin)
	{
		newlex();
		Arg0();
		PolizElem *temp=new PolizJoin;
		addpolizelem(temp);
	}
	else if (curlex->type==KeyWord&&curlex->lexnum==LexBuild)
	{
		newlex();
		Arg0();
		PolizElem *temp=new PolizBuild;
		addpolizelem(temp);
	}
	else if (curlex->type==KeyWord&&curlex->lexnum==LexTurn)
	{
		newlex();
		Arg0();
		PolizElem *temp=new PolizTurn;
		addpolizelem(temp);
	}
	else throw parserr("expected operator",curlex);
}

void parser::Exp()
{
	int k;
	PolizElem *temp;

	Exp1();
	if ((k=ComOp()))
	{
		Exp1();
		switch (k)
		{
			case '=':
				temp=new PolizFunEqu;
				break;
			case '>':
				temp=new PolizFunMore;
				break;
			case '<':
				temp=new PolizFunLess;
		}
		addpolizelem(temp);
	}
}

void parser::Exp1()
{
	int k;
	PolizElem *temp;

	Exp2();
	while ((k=Op1()))
	{
		Exp2();
		switch (k)
		{
			case '+':
				temp=new PolizFunPlus;
				break;
			case '-':
				temp=new PolizFunMinus;
				break;
			case '|':
				temp=new PolizFunOr;
		}
		addpolizelem(temp);
	}
}

void parser::Exp2()
{
	int k;
	PolizElem *temp;

	Exp3();
	while ((k=Op2()))
	{
		Exp3();
		switch (k)
		{
			case '*':
				temp=new PolizFunMultipl;
				break;
			case '/':
				temp=new PolizFunDivision;
				break;
			case '%':
				temp=new PolizFunPercent;
				break;
			case '&':
				temp=new PolizFunAnd;
		}
		addpolizelem(temp);
	}
}

void parser::Exp3()
{
	if (curlex->type==Variable)
	{
		PolizElem *temp=new PolizVarAddr(curlex->lexstr,0);
		addpolizelem(temp);
		newlex();
		Array(false);
		temp=new PolizVar;
		addpolizelem(temp);
	}
	else if (curlex->type==Number)
	{
		PolizElem *temp=new PolizInt(curlex->lexnum);
		addpolizelem(temp);
		newlex();
	}
	else if (curlex->type==Function)
	{
		FuncClass temp;
		char *str=curlex->lexstr;

		if (temp.getnum(str))
		{
			newlex();
			Arg1();
			PolizElem *temp=new PolizFunction1(str);
			addpolizelem(temp);
		}
		else
		{
			newlex();
			Arg0();
			PolizElem *temp=new PolizFunction0(str);
			addpolizelem(temp);
		}
	}
	else if (curlex->type==Operation&&curlex->lexnum=='-')
	{
		PolizElem *temp=new PolizInt(0);
		addpolizelem(temp);
		newlex();
		Exp3();
		temp=new PolizFunMinus;
		addpolizelem(temp);
	}
	else if (curlex->type==Operation&&curlex->lexnum=='!')
	{
		newlex();
		Exp3();
		PolizElem *temp=new PolizFunNeg;
		addpolizelem(temp);
	}
	else if (curlex->type==Bracket&&curlex->lexnum=='(')
	{
		newlex();
		Exp();
		if (curlex->type==Bracket&&curlex->lexnum==')')
			newlex();
		else throw parserr("expected `)'",curlex);
	}
	else throw parserr("expected expression",curlex);
}

int parser::ComOp()
{
	if (curlex->type==Equal)
	{
		newlex();
		return '=';
	}
	else if (curlex->type==Operation&&
															(curlex->lexnum=='<'||curlex->lexnum=='>'))
	{
		int temp=curlex->lexnum;
		newlex();
		return temp;
	}
	return 0;
}

int parser::Op1()
{
	if (curlex->type==Operation&&
					(curlex->lexnum=='-'||curlex->lexnum=='|'||curlex->lexnum=='+'))
	{
		int temp=curlex->lexnum;
		newlex();
		return temp;
	}
	return 0;
}

int parser::Op2()
{
	if (curlex->type==Operation&&
													(curlex->lexnum=='*'||curlex->lexnum=='/'||
													curlex->lexnum=='%'||curlex->lexnum=='&'))
	{
		int temp=curlex->lexnum;
		newlex();
		return temp;
	}
	return 0;
}

void parser::Arg0()
{
	if (curlex->type==Bracket&&curlex->lexnum=='(')
	{
		newlex();
		if (curlex->type==Bracket&&curlex->lexnum==')')
			newlex();
		else throw parserr("expected `)'",curlex);
	}
	else throw parserr("expected `('",curlex);
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

bool parser::VarArg()
{
	if (curlex->type==Operation&&curlex->lexnum=='=')
	{
		newlex();
		Exp1();
		PolizElem *temp=new PolizFunAssig;
		addpolizelem(temp);
		return true;
	}
	return false;
}

bool parser::Array(bool set)
{
	if (curlex->type==Bracket&&curlex->lexnum=='[')
	{
		newlex();
		Exp1();
		if (curlex->type==Bracket&&curlex->lexnum==']')
			newlex();
		else throw parserr("expected `]'",curlex);
		PolizElem *temp=new PolizIdx(set);
		addpolizelem(temp);
		return true;
	}
	return false;
}

void parser::ArgList()
{
	if (curlex->type==Bracket&&curlex->lexnum=='(')
	{
		newlex();
		ArgPrint();
		while (curlex->type==Divider&&curlex->lexnum==',')
		{
			newlex();
			ArgPrint();
		}
		if (curlex->type==Bracket&&curlex->lexnum==')')
			newlex();
		else throw parserr("expected `)'",curlex);
	}
	else throw parserr("expected `('",curlex);
}

void parser::ArgPrint()
{
	if (curlex->type==String)
	{
		PolizElem *temp=new PolizString(curlex->lexstr);
		addpolizelem(temp);
		newlex();
	}
	else
		Exp1();
}

void parser::addpolizelem(PolizElem *elem)
{
	if (!poliz)
		curpolizelem=poliz=new PolizItem(elem);
	else
		curpolizelem=curpolizelem->next=new PolizItem(elem);
}
