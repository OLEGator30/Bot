#include <string.h>
#include <stdio.h>
#include "errors.hpp"
#include "../buffer/buffer.hpp"

errors::errors(const char* str)
{
	int len=strlen(str);

	msg=new char[len+1];
	strcpy(msg,str);
}

errors::~errors()
{
	delete[] msg;
}

void errors::print() const
{
	classbuf tmp;
	char str[5];

	sprintf(str,"line %d: ",line);
	tmp.write(str);
	tmp.write(msg);
	printf("%s",tmp.read());
}
