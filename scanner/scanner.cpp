#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "scanner.hpp"

lexlist* scanner::run(int fd)
{
	int i;
	char str[128];

	begin=end=new lexlist; // first node is empty!
	lexlist* temp;
	for(;;)
	{
		if ((i=read(fd,str,128))==0) break;
		for (int j=0;j<i;++j)
		{
			temp=myfsm.newchar(str[j],line);
			if (temp) end=addnewlex(end,temp);
			if (str[j]=='\n') ++line;
		}
	}
	temp=myfsm.newchar(0,line);
	if (temp) end=addnewlex(end,temp);
	end=begin->next;
	// delete begin; // remove first node
	return end;
}

lexlist* scanner::addnewlex(lexlist *list,lexlist* p)
{
	list->next=p;
	if (p->next) p=p->next;
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
	if ((c=='(')||(c==')')||(c=='[')||(c==']')||(c=='{')||(c=='}'))
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
	else if (c=='?') { state=Fun; return 0; }
	else if (c=='@') { state=Lab; return 0; }
	else if ((c>='0')&&(c<='9')) { state=Num; buffer.write(c); return 0; }
	else if (c=='"') { state=Str; return 0; }
	else if (inchar(c)) { state=Key; buffer.write(c); return 0; }
	//       ^ use only after numbers!!
	else if (inoperations(c)) { temp=addnewlex(c,Operation); return temp; }
	else if (inbrackets(c)) { temp=addnewlex(c,Bracket); return temp; }
	else if (individers(c)) { temp=addnewlex(c,Divider); return temp; }
	else if (inspaces(c)) return 0;
	else if (c=='=') { state=Equ; return 0; }
	char s[32];
	sprintf(s,"%c: unexpected character\n",c);
	throw scanerr(s,line);
}

lexlist* fsm::numproc(char c)
{
	lexlist *temp;

	if ((c>='0')&&(c<='9')) { buffer.write(c); return 0; }
	else if (inoperations(c))
	{
		temp=addnewlex(Number);
		temp->next=addnewlex(c,Operation);
		return temp;
	}
	else if (inbrackets(c))
	{
		temp=addnewlex(Number);
		temp->next=addnewlex(c,Bracket);
		return temp;
	}
	else if (individers(c))
	{
		temp=addnewlex(Number);
		temp->next=addnewlex(c,Divider);
		return temp;
	}
	else if (inspaces(c)) { temp=addnewlex(Number); return temp; }
	else if (c=='=') { temp=addnewlex(Number); state=Equ; return temp; }
	buffer.write(c);
	buffer.write(": unexpected sequence of charecters\n");
	throw scanerr(buffer.read(),line);
}

lexlist* fsm::funproc(char c)
{
	lexlist *temp;

	if (inchar(c)) { buffer.write(c); return 0; }
	else if (inspaces(c)) { temp=addnewlex(Function); return temp; }
	else if (inbrackets(c))
	{
		temp=addnewlex(Function);
		temp->next=addnewlex(c,Bracket);
		return temp;
	}
	buffer.write(c);
	buffer.write(": unexpected sequence of charecters\n");
	throw scanerr(buffer.read(),line);
}

lexlist* fsm::varproc(char c)
{
	lexlist *temp;

	if (inchar(c)) { buffer.write(c); return 0; }
	else if (inoperations(c))
	{
		varitem *item=new varitem(buffer.read());
		table.addnewvar(item);
		temp=addnewlex(Variable);
		temp->next=addnewlex(c,Operation);
		return temp;
	}
	else if (individers(c))
	{
		varitem *item=new varitem(buffer.read());
		table.addnewvar(item);
		temp=addnewlex(Variable);
		temp->next=addnewlex(c,Divider);
		return temp;
	}
	else if (inbrackets(c))
	{
		varitem *item=new varitem(buffer.read());
		table.addnewvar(item);
		temp=addnewlex(Variable);
		temp->next=addnewlex(c,Bracket);
		return temp;
	}
	else if (inspaces(c))
	{
		varitem *item=new varitem(buffer.read());
		table.addnewvar(item);
		temp=addnewlex(Variable);
		return temp;
	}
	else if (c=='=')
	{
		varitem *item=new varitem(buffer.read());
		table.addnewvar(item);
		temp=addnewlex(Variable);
		state=Equ;
		return temp;
	}
	buffer.write(c);
	buffer.write(": unexpected sequence of charecters\n");
	throw scanerr(buffer.read(),line);
}

lexlist* fsm::labproc(char c)
{
	lexlist *temp;

	if (inchar(c)) { buffer.write(c); return 0; }
	else if (individers(c))
	{
		labitem *item=new labitem(buffer.read());
		table.addnewlab(item);
		temp=addnewlex(Label);
		temp->next=addnewlex(c,Divider);
		return temp;
	}
	else if (inspaces(c))
	{
		labitem *item=new labitem(buffer.read());
		table.addnewlab(item);
		temp=addnewlex(Label);
		return temp;
	}
	buffer.write(c);
	buffer.write(": unexpected sequence of charecters\n");
	throw scanerr(buffer.read(),line);
}

lexlist* fsm::keyproc(char c)
{
	lexlist *temp;

	if (inchar(c)) { buffer.write(c); return 0; }
	else if (individers(c))
	{
		temp=addnewlex(KeyWord);
		temp->next=addnewlex(c,Divider);
		return temp;
	}
	else if (inspaces(c)) { temp=addnewlex(KeyWord); return temp; }
	else if (inbrackets(c))
	{
		temp=addnewlex(KeyWord);
		temp->next=addnewlex(c,Bracket);
		return temp;
	}
	buffer.write(c);
	buffer.write(": unexpected sequence of charecters\n");
	throw scanerr(buffer.read(),line);
}

lexlist* fsm::equproc(char c)
{
	lexlist *temp;

	if (c=='=')	{ temp=addnewlex('=',Equal); return temp; }
	else if (inspaces(c)) { temp=addnewlex('=',Operation); return temp; }
	else if (inbrackets(c))
	{
		temp=addnewlex('=',Operation);
		temp->next=addnewlex(c,Bracket);
		return temp;
	}
	else if (inoperations(c))
	{
		temp=addnewlex('=',Operation);
		temp->next=addnewlex(c,Operation);
		return temp;
	}
	else if ((c>='0')&&(c<='9'))
	{temp=addnewlex('=',Operation); buffer.write(c); state=Num; return temp;}
	else if (c=='$') {temp=addnewlex('=',Operation); state=Var; return temp;}
	else if (c=='?') {temp=addnewlex('=',Operation); state=Fun; return temp;}
	buffer.write('='); buffer.write(c);
	buffer.write(": unexpected sequence of charecters\n");
	throw scanerr(buffer.read(),line);
}

lexlist* fsm::newchar(int c,int ln)
{
	line=ln;

	if (c==0)
		if (state!=Home)
			throw scanerr("missed \"\n",line);
		else
		{
			lexlist *tmp;
			tmp=addnewlex();
			return tmp;
		}
	else
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
				if (c=='"') { temp=addnewlex(String); return temp; }
				else { buffer.write(c); return 0; }
		}
	return 0; // g++ wants it...
}

lexlist* fsm::addnewlex(LexType type)
{
	lexlist *p;
	p=new lexlist;

	state=Home;
	p->type=type;
	p->line=line;
	if (type==KeyWord)
	{
		int i=0,k=sizeof(StrKeyWords)/sizeof(void*);

		for (;i<k;++i)
			if (strcmp(buffer.read(),StrKeyWords[i])==0) break;
		if (i==k)
		{
			buffer.write(": not a key word\n");
			throw scanerr(buffer.read(),line);
		}
		p->lexnum=i;
		buffer.free();
		return p;
	}
	else if (type==Number)
	{
		p->lexnum=strtoint(buffer.read());
		buffer.free();
		return p;
	}
	else if (strlen(buffer.read()))
	{
		p->lexstr=new char[strlen(buffer.read())+1];	
		strcpy(p->lexstr,buffer.read());
		buffer.free();
		return p;
	}
	else throw scanerr("empty name\n",line);
}

lexlist* fsm::addnewlex(char c,LexType type)
{
	lexlist *p;

	p=new lexlist;
	p->line=line;
	p->type=type;
	p->lexnum=c;
	state=Home;
	return p;
}

lexlist* fsm::addnewlex()
{
	lexlist *p;

	p=new lexlist;
	p->line=line;
	p->type=KeyWord;
	p->lexnum=sizeof(StrKeyWords)/sizeof(void*);
	return p;
}
