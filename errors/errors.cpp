#include <string.h>
#include <stdio.h>
#include "errors.hpp"

errors::~errors()
{
	delete[] msg;
}

scanerr::scanerr(const char* str,int ln)
{
	int len=strlen(str);

	msg=new char[len+1];
	strcpy(msg,str);
	line=ln;
}

void scanerr::print() const
{
	printf("%3.d: %s",line,msg);
}

parserr::parserr(const char* str,lexlist* list)
{
	int len=strlen(str);

	msg=new char[len+1];
	strcpy(msg,str);
	curlex=list;
}

void parserr::print() const
{
	printf("%3.d: %s, but ",curlex->line,msg);
	if (curlex->lexstr) printf("`%s' found\n",curlex->lexstr);
	else
	{
		if (curlex->type==KeyWord)
		{
			if (curlex->lexnum==(sizeof(StrKeyWords)/sizeof(void*)))
				printf ("`EOF' found\n");
			else
				printf("`%s' found\n",StrKeyWords[curlex->lexnum]);
			return;
		}
		if (curlex->type==Number) printf("`%d' found\n",curlex->lexnum);
		else
			if (curlex->type==Equal) printf("`==' found\n");
			else printf("`%c' found\n",curlex->lexnum);
	}
}

polizerr::polizerr(const char* str)
{
	int len=strlen(str);

	msg=new char[len+1];
	strcpy(msg,str);
}

void polizerr::print() const
{
	printf("%s\n",msg);
}
