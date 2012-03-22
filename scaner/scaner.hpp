#ifndef SCANER_HPP
#define SCANER_HPP

#include "../other/buffer.hpp"

enum states { Home, Number, Ident, Key, Assign, String };

struct lexlist
{
	classbuf lexeme;
	char type;
	int line;
	lexlist* next;

	lexlist(): type(0), line(0), next(0) {}
};

class scaner
{
	states state;
	classbuf buffer;
	int symb;

	public:

	scaner(): symb(0) {};
	lexlist* run(int);
};

#endif
