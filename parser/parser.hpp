#ifndef PARSER_HPP
#define PARSER_HPP

#include "../errors/errors.hpp"
#include "../scaner/scaner.hpp"

class parser
{
	lexlist *curlex;

	void newlex();
	void Prog();
	void Block();

	public:

	parser(){}
	void run(lexlist*);
};

#endif
