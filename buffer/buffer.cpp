#include "buffer.hpp"
#include <string.h>

classbuf::classbuf()
{
	size=32;
	weight=0;
	buffer=new char[size];
	buffer[0]=0;
}

classbuf::~classbuf() { delete[] buffer; }

void classbuf::doublebuf()
{
	char* temp;
	
	size*=2;
	temp=new char[size];
	strcpy(temp,buffer);
	delete[] buffer;
	buffer=temp;
}

void classbuf::write(char *str)
{
	int len=strlen(str);

	while ((size-weight-1)<len) doublebuf();
	strcpy(buffer+weight,str);
	weight+=len;
	buffer[weight]=0;
}

void classbuf::write(char c)
{
	if ((size-weight-1)==0) doublebuf();
	buffer[weight]=c;
	++weight;
	buffer[weight]=0;
}

char *classbuf::read() const
{
	char *str;
	
	str=new char[weight+1];
	return strcpy(str,buffer);
}

void classbuf::free()
{	
	if (size>32)
	{
		delete[] buffer;
		size=32;
		buffer=new char[size];
	}
	weight=0;
	buffer[0]=0;
}
