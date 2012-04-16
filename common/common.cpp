#include "common.hpp"
#include <string.h>
#include <stdio.h>

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
