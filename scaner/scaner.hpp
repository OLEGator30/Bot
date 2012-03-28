#ifndef SCANER_HPP
#define SCANER_HPP

#include "../buffer/buffer.hpp"

extern const char* KeyWords[9];

enum states { Home, Num, Ident, Key, Assign, Str };

enum lextypes { Number, Identifier, KeyWord, String, Divider };

struct lexlist
{
	classbuf lexeme;
	lextypes type;
	int line;
	lexlist* next;
};

class scaner
{
	states state;
	classbuf buffer;
	int line;

	void addnewlex(lexlist*,lextypes);
	void addnewlex(lexlist*,char);
	void homeproc(lexlist*,char);
	void numproc(lexlist*,char);
	void identproc(lexlist*,char);
	void keyproc(lexlist*,char);
	void assignproc(lexlist*,char);

	public:

	scaner(): state(Home), line(1) {};
	lexlist* run(int);
};

#endif
