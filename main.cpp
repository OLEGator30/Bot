#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>	
#include "scaner/scaner.hpp"

const char *states2str(lextypes type)
{
	if (type==Number) return "number";
	if (type==Identifier) return "identifier";
	if (type==KeyWord) return "key word";
	if (type==String) return "string";
	if (type==Divider) return "divider";
	return "FAIL!";
}

int main()
{
	scaner scan;
	int fd;
	lexlist *list;

	fd=open("input.txt",O_RDONLY);
	list=scan.run(fd);
	if (list==0) return 1;
	while (list->next!=NULL)
	{
		lexlist *p;
		p=list;
		list=list->next;
		printf("%s %s (%d)\n",
							(list->lexeme).read(),states2str(list->type),list->line);
		delete p;
	}
	close(fd);
	return 0;
}

