#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "scaner.hpp"
#include "../errors/errors.hpp"

const char* StrKeyWords[9]=
{ "if","then","goto","print","buy","sell","prod","build","endturn" };

const char* states2str(LexType type)
{
	if (type==String) return "string";
	if (type==Divider) return "divider";
	if (type==Function) return "function";
	if (type==Variable) return "variable";
	if (type==Label) return "label";
	if ((type==Operation)||(type=Equal)) return "operation";
	if (type==Bracket) return "bracket";
	return "FAIL!"; // g++ wants it...
}

int strtoint(char *s)
{
	unsigned int i=0,k=0;

	for (;i<strlen(s);i++)
	{
		if ((s[i]>='0')&&(s[i]<='9')) k=k*10+s[i]-'0';
		else return (-1);
	}
	return k;
}

lexlist::lexlist() { lexstr=0; lexnum=0; next=0; }

void lexlist::print()
{
	printf("%3.d: ",line);
	if (lexstr) printf("%s  (%s)\n",lexstr,states2str(type));
	else
	{
		if (type==KeyWord)
		{
			printf("%s  (key word)\n",StrKeyWords[lexnum]);
			return;
		}
		if (type==Number) printf("%d  (number)\n",lexnum);
		else
			if (type==Equal) printf("==  (equal)\n");
			else printf("%c  (%s)\n",lexnum,states2str(type));
	}
}

lexlist* scaner::run(int fd)
{
	int i;
	char str[128];

	begin=end=new lexlist; // first node is empty!
	try
	{
		lexlist* temp;
		for(;;)
		{
			if ((i=read(fd,str,128))==0) break;
			for (int j=0;j<i;++j)
			{
				temp=myfsm.newchar(str[j]);
				if (temp) end=addnewlex(end,temp,line);
				if (str[j]=='\n') ++line;
			}
		}
		temp=myfsm.newchar(' ');
		if (temp) end=addnewlex(end,temp,line);		
	}
	catch (errors &err)
	{
		err.line=line;
		err.print();
		exit(1);
	}
	end=begin->next;
	delete begin; // remove first node
	return end;
}

lexlist* scaner::addnewlex(lexlist *list,lexlist* p,int line)
{
	p->line=line;	
	list->next=p;
	if (p->next)
	{
		p=p->next;
		p->line=line;
	}
	return p;
}

bool fsm::inoperations(char c)
{
	if ((c=='+')||(c=='-')||(c=='*')||(c=='/')||(c=='%')||(c=='<')
												||(c=='>')||(c=='&')||(c=='|')||(c=='!'))
		return true;
	return false;
}

bool fsm::inbrackets(char c)
{
	if ((c=='(')||(c==')')||(c=='[')||(c==']'))
		return true;
	return false;
}

bool fsm::individers(char c)
{
	if ((c==';')||(c==',')||(c==':')) return true;
	return false;
}

bool fsm::inspaces(char c)
{
	if ((c==' ')||(c=='\n')||(c=='\t')) return true;
	return false;
}

bool fsm::inchar(char c)
{
	if (((c>='A')&&(c<='Z'))||((c>='a')&&(c<='z'))||((c>='0')&&(c<='9'))
																								||(c=='_'))
		return true;
	return false;
}

lexlist* fsm::homeproc(char c)
{
	lexlist *temp;

	if (c=='$') { state=Var; return 0; }
	if (c=='?') { state=Fun; return 0; }
	if (c=='@') { state=Lab; return 0; }
	if ((c>='0')&&(c<='9')) { state=Num; buffer.write(c); return 0; }
	if (c=='"') { state=Str; return 0; }
	if (inchar(c)) { state=Key; buffer.write(c); return 0; }
	//  ^ use only after numbers!!
	if (inoperations(c)) { temp=addnewlex(c,Operation); return temp; }
	if (inbrackets(c)) { temp=addnewlex(c,Bracket); return temp; }
	if (individers(c)) { temp=addnewlex(c,Divider); return temp; }
	if (inspaces(c)) return 0;
	if (c=='=') { state=Equ; return 0; }
	char s[32];
	sprintf(s,"%c: unexpected character\n",c);
	throw errors(s);
}

lexlist* fsm::numproc(char c)
{
	lexlist *temp;

	if ((c>='0')&&(c<='9')) { buffer.write(c); return 0; }
	if (inoperations(c))
	{
		temp=addnewlex(buffer.read(),Number);
		temp->next=addnewlex(c,Operation);
		return temp;
	}
	if (individers(c))
	{
		temp=addnewlex(buffer.read(),Number);
		temp->next=addnewlex(c,Divider);
		return temp;
	}
	if (inspaces(c))
	{
		temp=addnewlex(buffer.read(),Number);
		return temp;
	}
	if (c=='=')
	{
		temp=addnewlex(buffer.read(),Number);
		state=Equ;
		return temp;
	}
	buffer.write(c);
	buffer.write(": unexpected sequence of charecters\n");
	throw errors(buffer.read());
}

lexlist* fsm::funproc(char c)
{
	lexlist *temp;

	if (inchar(c)) { buffer.write(c); return 0; }
	if (inoperations(c))
	{
		temp=addnewlex(buffer.read(),Function);
		temp->next=addnewlex(c,Operation);
		return temp;
	}
	if (individers(c))
	{
		temp=addnewlex(buffer.read(),Function);
		temp->next=addnewlex(c,Divider);
		return temp;
	}
	if (inspaces(c))
	{
		temp=addnewlex(buffer.read(),Function);
		return temp;
	}
	if (inbrackets(c))
	{
		temp=addnewlex(buffer.read(),Function);
		temp->next=addnewlex(c,Bracket);
		return temp;
	}
	buffer.write(c);
	buffer.write(": unexpected sequence of charecters\n");
	throw errors(buffer.read());
}

lexlist* fsm::varproc(char c)
{
	lexlist *temp;

	if (inchar(c)) { buffer.write(c); return 0; }
	if (inoperations(c))
	{
		temp=addnewlex(buffer.read(),Variable);
		temp->next=addnewlex(c,Operation);
		return temp;
	}
	if (individers(c))
	{
		temp=addnewlex(buffer.read(),Variable);
		temp->next=addnewlex(c,Divider);
		return temp;
	}
	if (inspaces(c))
	{
		temp=addnewlex(buffer.read(),Variable);
		return temp;
	}
	if (c=='=')
	{
		temp=addnewlex(buffer.read(),Variable);
		state=Equ;
		return temp;
	}
	buffer.write(c);
	buffer.write(": unexpected sequence of charecters\n");
	throw errors(buffer.read());
}

lexlist* fsm::labproc(char c)
{
	lexlist *temp;

	if (inchar(c)) { buffer.write(c); return 0; }
	if (inoperations(c))
	{
		temp=addnewlex(buffer.read(),Label);
		temp->next=addnewlex(c,Operation);
		return temp;
	}
	if (individers(c))
	{
		temp=addnewlex(buffer.read(),Label);
		temp->next=addnewlex(c,Divider);
		return temp;
	}
	if (inspaces(c)) { temp=addnewlex(buffer.read(),Label); return temp; }
	buffer.write(c);
	buffer.write(": unexpected sequence of charecters\n");
	throw errors(buffer.read());
}

lexlist* fsm::keyproc(char c)
{
	lexlist *temp;

	if (inchar(c)) { buffer.write(c); return 0; }
	if (inoperations(c))
	{
		temp=addnewlex(buffer.read(),KeyWord);
		temp->next=addnewlex(c,Operation);
		return temp;
	}
	if (individers(c))
	{
		temp=addnewlex(buffer.read(),KeyWord);
		temp->next=addnewlex(c,Divider);
		return temp;
	}
	if (inspaces(c))
	{
		temp=addnewlex(buffer.read(),KeyWord);
		return temp;
	}
	if (inbrackets(c))
	{
		temp=addnewlex(buffer.read(),KeyWord);
		temp->next=addnewlex(c,Bracket);
		return temp;
	}
	buffer.write(c);
	buffer.write(": unexpected sequence of charecters\n");
	throw errors(buffer.read());
}

lexlist* fsm::equproc(char c)
{
	lexlist *temp;

	if (c=='=')
	{
		temp=addnewlex('=',Equal);
		state=Home;
		return temp;
	}
	if (inspaces(c))
	{
		temp=addnewlex('=',Operation);
		state=Home;
		return temp;
	}
	if (inbrackets(c))
	{
		temp=addnewlex('=',Operation);
		temp->next=addnewlex(c,Bracket);
		return temp;
	}
	if ((c>='0')&&(c<='9'))
	{
		temp=addnewlex('=',Operation);
		buffer.write(c);
		state=Num;
		return temp;
	}
	if (c=='$')
	{
		temp=addnewlex('=',Operation);
		state=Var;
		return temp;
	}
	if (c=='?')
	{
		temp=addnewlex('=',Operation);
		state=Fun;
		return temp;
	}
	buffer.write('='); buffer.write(c);
	buffer.write(": unexpected sequence of charecters\n");
	throw errors(buffer.read());
}

lexlist* fsm::newchar(char c)
{
	switch (state)
	{
		lexlist *temp;

		case Home: return homeproc(c);
		case Num: return numproc(c);
		case Fun: return funproc(c);
		case Var: return varproc(c);
		case Lab: return labproc(c);
		case Key: return keyproc(c);
		case Equ: return equproc(c);
		case Str:
			if (c=='"') { temp=addnewlex(buffer.read(),String); return temp; }
			else { buffer.write(c); return 0; }
	}
	return 0; // g++ wants it...
}

lexlist* fsm::addnewlex(char* str,LexType type)
{
	lexlist *p;
	p=new lexlist;

	state=Home;
	p->type=type;
	if (type==KeyWord)
	{
		int i=0;		

		for (;i<9;++i)
			if (strcmp(buffer.read(),StrKeyWords[i])==0) break;
		if (i==9)
		{
			buffer.write(": not a key word\n");
			throw errors(buffer.read());
		}
		p->lexnum=i;
		buffer.free();
		return p;
	}
	if (type==Number)
	{
		p->lexnum=strtoint(str);
		buffer.free();
		return p;
	}
	p->lexstr=new char[strlen(buffer.read())+1];	
	strcpy(p->lexstr,buffer.read());
	buffer.free();
	return p;
}

lexlist* fsm::addnewlex(char c,LexType type)
{
	lexlist *p;

	p=new lexlist;
	p->type=type;
	p->lexnum=c;
	state=Home;
	return p;
}
