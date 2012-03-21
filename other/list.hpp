#ifndef LIST_HPP
#define LIST_HPP

#include "buffer.hpp"

struct lexlist
{
	classbuf lexeme;
	char type;
	int line;
	lexlist* next;

	lexlist(): type(0), line(0), next(0) {}
};

#endif
