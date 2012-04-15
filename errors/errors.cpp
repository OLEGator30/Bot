#include <string.h>
#include <stdio.h>
#include "errors.hpp"
#include "../buffer/buffer.hpp"

errors::errors(const char* str,int ln)
{
	int len=strlen(str);

	msg=new char[len+1];
	strcpy(msg,str);
	line=ln;
}

errors::~errors()
{
	delete[] msg;
}

void errors::print() const
{
	printf("%3.d: %s",line,msg);
}

scanerr::scanerr(const char* str,int line):errors(str,line) {}

parserr::parserr(const char* str,int line):errors(str,line) {}
