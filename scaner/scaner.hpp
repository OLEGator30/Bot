#ifndef SCANER_HPP
#define SCANER_HPP

#include "../other/buffer.hpp"
#include "../other/list.hpp"

enum states	{ Home, Number, Ident, Key, Assign, String, };

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
