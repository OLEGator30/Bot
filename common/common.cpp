#include <stdio.h>
#include <string.h>
#include "common.hpp"
#include "../errors/errors.hpp"

Tables table;

const char* StrKeyWords[9]=
{ "if","goto","print","int","buy","sell","prod","build","endturn" };

const char* states2str(LexType type)
{
	if (type==String) return "string";
	else if (type==Divider) return "divider";
	else if (type==Function) return "function";
	else if (type==Variable) return "variable";
	else if (type==Label) return "label";
	else if (type==Operation) return "operation";
	else if (type==Bracket) return "bracket";
	return "FAIL!";
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

lexlist::lexlist(): lexstr(0), lexnum(0), next(0) {}

void lexlist::print()
{
	printf("%3.d: ",line);
	if (lexstr) printf("%s (%s)\n",lexstr,states2str(type));
	else
	{
		if (type==KeyWord)
		{
			printf("%s (key word)\n",StrKeyWords[lexnum]);
			return;
		}
		if (type==Number) printf("%d (number)\n",lexnum);
		else
			if (type==Equal) printf("== (equal)\n");
			else printf("%c (%s)\n",lexnum,states2str(type));
	}
}

PolizItem::PolizItem(PolizElem *e): elem(e), next(0) {}

labitem::labitem(char *str): decl(false), val(0), next(0)
{
	if (str)
	{
		name=new char[strlen(str)+1];
		strcpy(name,str);
	}
	else
		name=0;
}

labitem::~labitem()
{
	if (name) delete[] name;
}

varitem::varitem(char *str): decl(false), next(0)
{
	name=new char[strlen(str)+1];
	strcpy(name,str);
}

varitem::~varitem()
{
	if (name) delete[] name;
}

Tables::Tables(): lablist(0), varlist(0) {}

labitem* Tables::foundlab(char *str)
{
	if (lablist)
	{
		labitem *temp=lablist;

		while (strcmp(temp->name,str))
		{
			temp=temp->next;
			if (!temp)
				return 0;
		}
		return temp;
	}
	return 0;
}

varitem* Tables::foundvar(char *str)
{
	if (varlist)
	{
		varitem *temp=varlist;

		while (strcmp(temp->name,str))
		{
			temp=temp->next;
			if (!temp)
				return 0;
		}
		return temp;
	}
	return 0;
}

void Tables::addnewlab(labitem *item)
{
	if (lablist)
	{
		if (!foundlab(item->name))
		{
			labitem *temp=lablist;
			while (temp->next)
				temp=temp->next;
			temp->next=item;
		}
		else
			delete item;
	}
	else
		lablist=item;
}

void Tables::addnewvar(varitem *item)
{
	if (varlist)
	{
		if (!foundvar(item->name))
		{
			varitem *temp=varlist;
			while (temp->next)
				temp=temp->next;
			temp->next=item;
		}
		else
			delete item;
	}
	else
		varlist=item;
}

void Tables::decllab(char *str,PolizItem *val)
{
	labitem *temp=foundlab(str);
	if (temp->decl)
		throw polizerr("redeclaration of label");
	temp->decl=true;
	temp->val=val;
}

void Tables::declvar(char *str)
{
	varitem *temp=foundvar(str);
	if (temp->decl)
		throw polizerr("rediclaration of variable");
	temp->decl=true;
}

void Tables::check()
{
	varitem *tempvar=varlist;
	while (tempvar)
	{
		if (tempvar->decl)
			tempvar=tempvar->next;
		else
			throw polizerr("var not decl");
	}
	labitem *templab=lablist;
	while (templab)
	{
		if (templab->decl)
			templab=templab->next;
		else
			throw polizerr("var not decl");
	}
}
