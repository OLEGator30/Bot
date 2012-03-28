#ifndef ERRORS_HPP
#define ERRORS_HPP

class errors
{
	int line;
	char* msg;

	public:

	errors(int,const char*);
	~errors();
	void print() const;
};

#endif
