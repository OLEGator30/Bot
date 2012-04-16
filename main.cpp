#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>	
#include "common/common.hpp"
#include "scanner/scanner.hpp"
#include "parser/parser.hpp"

int main()
{
	scaner scan;
	parser pars;
	int fd;
	lexlist *list;

	if ((fd=open("input",O_RDONLY))==-1)
	{
		perror("input");
		exit(1);
	}
	try
	{
		list=scan.run(fd); // launch scaner
		close(fd);
		pars.run(list); // launch parser
	}
	catch (const scanerr &err)
	{
		err.print();
		return 1;
	}
	catch (const parserr &err)
	{
		err.print();
		return 1;
	}

	while (list->next) // print except last node
	{
		list->print();
		lexlist *p=list;
		list=list->next;
		delete p;
	}
	delete list;
	return 0;
}

