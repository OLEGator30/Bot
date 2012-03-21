#ifndef BUFFER_HPP
#define BUFFER_HPP

class classbuf
{
	char *buffer;
	int size;
	int weight;

	public:

	classbuf();
	~classbuf() { delete[] buffer; }
};

#endif
