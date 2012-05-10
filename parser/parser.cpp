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
				PolizOpGoFalse *temp1=new PolizOpGoFalse;
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
				PolizVarAddr *temp=new PolizVarAddr(curlex->lexstr,0);
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
		PolizVarAddr *temp1=new PolizVarAddr(curlex->lexstr,0);
		addpolizelem(temp1);
		newlex();
		Array(false);
		if (curlex->type==Operation&&curlex->lexnum=='=')
		{
			newlex();
			Exp1();
			PolizFunAssig *temp2=new PolizFunAssig;
			addpolizelem(temp2);
		}
		else parserr("expected `='",curlex);
	}
	else if (curlex->type==KeyWord&&curlex->lexnum==LexGoto)
	{
		newlex();
		if (curlex->type==Label)
		{
			PolizLabel *temp3=new PolizLabel(curlex->lexstr); // ??
			addpolizelem(temp3);
			PolizOpGo *temp4=new PolizOpGo;
			addpolizelem(temp4);
			newlex();
		}
		else parserr("name of label expected",curlex);
	}
	else if (curlex->type==KeyWord&&curlex->lexnum==LexPrint)
	{
		PolizPrintEnd *temp1=new PolizPrintEnd;
		addpolizelem(temp1);
		newlex();
		ArgList();
		PolizPrint *temp2=new PolizPrint;
		addpolizelem(temp2);
	}
	else if (curlex->type==KeyWord&&curlex->lexnum==LexSell)
	{
		newlex();
		Arg2();
		PolizSell *temp=new PolizSell; // ???
		addpolizelem(temp);
	}
	else if (curlex->type==KeyWord&&curlex->lexnum==LexBuy)
	{
		newlex();
		Arg2();
		PolizBuy *temp=new PolizBuy; // ???
		addpolizelem(temp);
	}
	else if (curlex->type==KeyWord&&curlex->lexnum==LexProd)
	{
		newlex();
		Arg1();
		PolizProd *temp=new PolizProd; // ???
		addpolizelem(temp);
	}
	else if (curlex->type==KeyWord&&curlex->lexnum==LexBuild)
	{
		newlex();
		PolizBuild *temp=new PolizBuild; // ???
		addpolizelem(temp);
	}
	else if (curlex->type==KeyWord&&curlex->lexnum==LexEndturn)
	{
		newlex();
		PolizTurn *temp=new PolizTurn; // ???
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
		PolizVarAddr *temp1=new PolizVarAddr(curlex->lexstr,0);
		addpolizelem(temp1);
		newlex();
		Array(false);
		PolizVar *temp2=new PolizVar;
		addpolizelem(temp2);
	}
	else if (curlex->type==Number)
	{
		PolizInt *temp=new PolizInt(curlex->lexnum);
		addpolizelem(temp);
		newlex();
	}
	/*
	else if (curlex->type==Function)
	{
		newlex();
	}
	*/
	else if (curlex->type==Operation&&curlex->lexnum=='-')
	{
		PolizInt *temp1=new PolizInt(0);
		addpolizelem(temp1);
		newlex();
		Exp3();
		PolizFunMinus *temp2=new PolizFunMinus;
		addpolizelem(temp2);
	}
	else if (curlex->type==Operation&&curlex->lexnum=='!')
	{
		newlex();
		Exp3();
		PolizFunNeg *temp=new PolizFunNeg;
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
		PolizFunAssig *temp=new PolizFunAssig;
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
		PolizIdx *temp=new PolizIdx(set);
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
		PolizString *temp=new PolizString(curlex->lexstr);
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
	{
		curpolizelem->next=new PolizItem(elem);
		curpolizelem=curpolizelem->next;
	}
}
