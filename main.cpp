#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>	
#include "scaner/scaner.hpp"

int main()
{
	scaner scan;
	int fd;
	lexlist *list;

	if ((fd=open("input",O_RDONLY))==-1)
	{
		perror("input");
		exit(1);
	}
	list=scan.run(fd); // launch scanner

	while (list)
	{
		list->print();
		lexlist *p=list;
		list=list->next;
		delete p;
	}

	close(fd);
	return 0;
}

