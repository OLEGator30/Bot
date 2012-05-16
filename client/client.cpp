#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "client.hpp"

sock::sock()
{
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket");
		exit(1);
	}
}

void sock::joinserv(int port)
{
	struct sockaddr_in addr;

	addr.sin_family=AF_INET;
	addr.sin_port=htons(port);
	if(!inet_aton("127.0.0.1",&(addr.sin_addr)))
	{
		perror("inet_aton");
		exit(1);
	}
	if(0!=connect(sockfd,(struct sockaddr*)&addr,sizeof(addr)))
	{
		perror("connect");
		exit(1);
	}
}

int sock::getfd()
{
	return sockfd;
}
