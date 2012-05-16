#ifndef CLIENT_HPP
#define CLIENT_HPP

class sock
{
	int sockfd;

	public:

	sock();
	void joinserv(int);
	int getfd();
};

#endif
