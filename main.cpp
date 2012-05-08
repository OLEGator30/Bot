#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>	
#include "common/common.hpp"
#include "scanner/scanner.hpp"
#include "parser/parser.hpp"
#include "poliz/poliz.hpp"

int main()
{
	scanner scan;
	parser pars;
	int fd;
	lexlist *list;
	PolizItem *poliz, *stack=0, *curcmd;

	if ((fd=open("input",O_RDONLY))==-1)
	{
		perror("input");
		exit(1);
	}
	try
	{
		list=scan.run(fd); // launch scanner
		close(fd);
		poliz=curcmd=pars.run(list); // launch parser
		table.check();
		while (curcmd)
		{
			PolizElem *temp=curcmd->elem;
			temp->Evaluate(&stack,poliz,&curcmd);
		}
	}
	catch (const errors &err)
	{
		err.print();
		return 1;
	}
	return 0;
}

