#ifndef ERRORS_HPP
#define ERRORS_HPP

class errors
{
	char* msg;
	int line;

	public:

	errors(const char*,int);
	~errors();
	void print() const;
};

class scanerr : public errors
{
	public:

	scanerr(const char*,int);
	~scanerr() {}
};

class parserr : public errors
{
	public:

	parserr(const char*,int);
	~parserr() {}
};

#endif
