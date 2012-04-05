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

#endif
