#ifndef ERRORS_HPP
#define ERRORS_HPP

#include "../common/common.hpp"
#include "../scanner/scanner.hpp"
#include "../buffer/buffer.hpp"

class errors
{
	protected:
	
	char* msg;

	public:

	errors() {}
	virtual ~errors();
	virtual void print() const =0;
};

class scanerr : public errors
{
	int line;

	public:

	scanerr(const char*,int);
	virtual ~scanerr() {}
	virtual void print() const;
};

class parserr : public errors
{
	lexlist* curlex;

	public:

	parserr(const char*,lexlist*);
	virtual ~parserr() {}
	virtual void print() const;
};

class polizerr : public errors
{
	public:

	polizerr(const char*);
	virtual ~polizerr() {}
	virtual void print() const;
};

#endif
