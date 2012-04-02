#ifndef ERRORS_HPP
#define ERRORS_HPP

class errors
{
	char* msg;

	public:

	int line;

	errors(const char*);
	~errors();
	void print() const;
};

#endif
