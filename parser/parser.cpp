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
		throw parserr("EOF\n",curlex->line);
}

void parser::Block()
{
	if ((curlex->type==Bracket)&&(curlex->lexnum=='{'))
	{
		// temp
		do
		{
			newlex();
		}while ((curlex->type!=KeyWord)||(curlex->lexnum!=sizeof(StrKeyWords)));
		// temp
	}
	else throw parserr("expexted `}'\n",curlex->line);
}
