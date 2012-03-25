#ifndef BUFFER_HPP
#define BUFFER_HPP

class classbuf
{
	char *buffer;
	int size;
	int weight;

	void doublebuf();

	public:

	classbuf();
	~classbuf();
	void write(char*);
	void write(char);
	char* read() const;
	void free();
};

#endif
