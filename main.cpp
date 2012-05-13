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

int main(int argc,char **argv)
{
	scanner scan;
	parser pars;
	int fd;
	lexlist *list;
	PolizItem *stack=0, *curcmd, *poliz;

	if (argc==2)
	{
		if ((fd=open(argv[1],O_RDONLY))==-1)
		{
			perror(argv[1]);
			exit(1);
		}
		try
		{
			list=scan.run(fd); // launch scanner
			close(fd);
			poliz=curcmd=pars.run(list); // launch parser
			table.check();
			// printpoliz(poliz);
			while (curcmd)
			{
				PolizElem *temp=curcmd->elem;
				temp->Evaluate(&stack,&curcmd); // interpretation
			}
			freemem(poliz);
		}
		catch (const errors &err)
		{
			err.print();
			return 1;
		}
		printf("\n");
		return 0;
	}
	else
	{
		printf("wrong number of parametrs\n");
		return 1;
	}
}

