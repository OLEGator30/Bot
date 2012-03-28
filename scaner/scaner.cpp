//#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "scaner.hpp"
#include "../errors/errors.hpp"

const char* KeyWords[9]=
{
	"if","then","goto","print","buy","sell","prod","build","endturn"
};

lexlist* scaner::run(int fd)
{
	lexlist *list;
	int c;

	// fd->0
	list=new lexlist;
	list->next=0;
	try
	{
		while ((c=getchar())!=EOF)
		{
			switch (state)
			{
				case Home:
					homeproc(list,c);
					break;
				case Num:
					numproc(list,c);
					break;
				case Ident:
					identproc(list,c);
					break;
				case Key:
					keyproc(list,c);
					break;
				case Assign:
					assignproc(list,c);
					break;
				case Str:
					if (c=='"') addnewlex(list,String);
					else buffer.write(c);
			}
			if (c=='\n') ++line;
		}
		if (state!=Home) throw errors(line,"unexpected end of file\n");
	}
	catch (const errors &err)
	{
		err.print();
		return 0;
	}
	return list;
}

void scaner::homeproc(lexlist *list,char c)
{
	if ((c>='0')&&(c<='9'))
	{
		state=Num;
		buffer.write(c);
		return;
	}
	if ((c=='@')||(c=='$')||(c=='?'))
	{
		state=Ident;
		buffer.write(c);
		return;
	}
	if (((c>='A')&&(c<='Z'))||((c>='a')&&(c<='z')))
	{
		state=Key;
		buffer.write(c);
		return;
	}
	if (c==':')
	{
		state=Assign;
		buffer.write(c);
		return;
	}
	if (c=='"')
	{
		state=Str;
		return;
	}
	if ((c=='+')||(c=='-')||(c=='*')||(c=='/')||(c=='%')||(c==';')
												||(c=='<')||(c=='>')||(c=='=')||(c==',')
												||(c=='(')||(c==')')||(c=='[')||(c==']'))
	{
		addnewlex(list,c);
		return;
	}
	if ((c=='\n')||(c==' ')||(c=='\t')) return;
	buffer.write(c);
	buffer.write(": unexpected character\n");
	throw errors(line,buffer.read());
}

void scaner::numproc(lexlist* list,char c)
{
	if ((c>='0')&&(c<='9'))
	{
		buffer.write(c);
		return;
	}
	if ((c==' ')||(c=='\n')||(c=='\t'))
	{
		addnewlex(list,Number);
		return;
	}
	if ((c=='+')||(c=='-')||(c=='*')||(c=='/')||(c=='%')||(c==';')
												||(c=='<')||(c=='>')||(c=='=')||(c==',')
												||(c=='(')||(c==')')||(c=='[')||(c==']'))
	{
		addnewlex(list,Number);
		addnewlex(list,c);
		return;
	}
	if (c==':')
	{
		state=Assign;
		buffer.write(c);
		return;
	}
	buffer.write(c);
	buffer.write(": unexpected sequence of characters\n");
	throw errors(line,buffer.read());
}

void scaner::identproc(lexlist *list,char c)
{
	if (((c>='A')&&(c<='Z'))||((c>='a')&&(c<='z'))||
														((c>='0')&&(c<='9'))||(c=='_'))
	{
		buffer.write(c);
		return;
	}
	if ((c==' ')||(c=='\n')||(c=='\t'))
	{
		addnewlex(list,Identifier);
		return;
	}
	if ((c=='+')||(c=='-')||(c=='*')||(c=='/')||(c=='%')||(c==';')
												||(c=='<')||(c=='>')||(c=='=')||(c==',')
												||(c=='(')||(c==')')||(c=='[')||(c==']'))
	{
		addnewlex(list,Identifier);
		addnewlex(list,c);
		return;
	}
	if (c==':')
	{
		addnewlex(list,Identifier);
		state=Assign;
		buffer.write(c);
		return;
	}
	buffer.write(c);
	buffer.write(": unexpected sequence of characters\n");
	throw errors(line,buffer.read());
}

void scaner::keyproc(lexlist* list,char c)
{
	if (((c>='A')&&(c<='Z'))||((c>='a')&&(c<='z')))
	{
		buffer.write(c);
		return;
	}
	if ((c==' ')||(c=='\n')||(c=='\t'))
	{
		addnewlex(list,KeyWord);
		return;
	}
	if ((c=='+')||(c=='-')||(c=='*')||(c=='/')||(c=='%')||(c==';')
												||(c=='<')||(c=='>')||(c=='=')||(c==',')
												||(c=='(')||(c==')')||(c=='[')||(c==']'))
	{
		addnewlex(list,KeyWord);
		addnewlex(list,c);
		return;
	}
	if (c==':')
	{
		addnewlex(list,KeyWord);
		state=Assign;
		buffer.write(c);
		return;
	}
	buffer.write(c);
	buffer.write(": unexpected sequence of characters\n");
	throw errors(line,buffer.read());
}

void scaner::assignproc(lexlist* list,char c)
{
	if (c=='=')
	{
		buffer.write(c);
		addnewlex(list,Divider);
		return;
	}
	if ((c>='0')&&(c<='9'))
	{
		addnewlex(list,':');
		state=Num;
		buffer.write(c);
		return;
	}
	if ((c=='@')||(c=='$')||(c=='?'))
	{
		addnewlex(list,':');
		state=Ident;
		return;
	}
	if (((c>='A')&&(c<='Z'))||((c>='a')&&(c<='z')))
	{
		addnewlex(list,':');
		state=Key;
		buffer.write(c);
		return;
	}
	if ((c=='\n')||(c==' ')||(c=='\t'))
	{
		addnewlex(list,':');
		state=Home;
		return;
	}
	buffer.write(c);
	buffer.write(": unexpected sequence of characters\n");
	throw errors(line,buffer.read());
}

void scaner::addnewlex(lexlist *list,lextypes type)
{
	lexlist *p;
	int i=0;

	if (type==KeyWord)
	{
		for (;i<9;++i)
			if (strcmp(buffer.read(),KeyWords[i])==0) break;
		if (i==9)
		{
			buffer.write(": not a key word\n");
			throw errors(line,buffer.read());
		}
	}
	p=new lexlist;
	p->next=0;
	p->line=line;
	p->type=type;
	(p->lexeme).write(buffer.read());
	while (list->next!=0)
		list=list->next;
	list->next=p;
	buffer.free();
	state=Home;
}

void scaner::addnewlex(lexlist *list,char c)
{
	lexlist *p;
	char temp[2];

	p=new lexlist;
	p->next=0;
	p->line=line;
	p->type=Divider;
	temp[0]=c;
	temp[1]=0;
	(p->lexeme).write(temp);
	while (list->next!=0)
		list=list->next;
	list->next=p;
	buffer.free();
	state=Home;
}
