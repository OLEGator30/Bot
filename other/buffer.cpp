#include "buffer.hpp"

classbuf::classbuf()
{
	size=32;
	weight=0;
	buffer=new char[size];
	buffer[0]=0;
}
