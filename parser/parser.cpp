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
	if ((curlex->type!=KeyWord)||(curlex->lexnum!=sizeof(StrKeyWords)))
		throw parserr("expected `EOF'",curlex);
}

void parser::Block()
{
	if ((curlex->type==Bracket)&&(curlex->lexnum=='{'))
		do
		{
			LabState();
		} while ((curlex->type==Bracket)&&(curlex->lexnum=='}'));
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
		// add label to LT
		newlex();
		if ((curlex->type==Divider)&&(curlex->lexnum==':'))
		{
			newlex();
			// all?
		}
		else throw parserr("expected `:'\n",curlex);
	}
}

void parser::State()
{
	if ((curlex->type==KeyWord)&&(curlex->lexnum==LexIf))
	{
		newlex();
		if ((curlex->type==Bracket)&&(curlex->lexnum=='('))
		{
			newlex();
			Exp();
			if ((curlex->type==Bracket)&&(curlex->lexnum==')'))
			{
				newlex();
				SimpleSt();
			}
			else throw parserr("expected `)'",curlex);
		}
		else throw parserr("expected `('",curlex);
	}
	else if ()
	{
		SimpleSt();
	}
	else throw parserr("expected `if'",curlex);
}
