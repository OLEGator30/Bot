#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include "functions.hpp"
#include "../common/common.hpp"
#include "../errors/errors.hpp"

FuncClass function;

Player::Player(int num): number(num), bdiff(0), sdiff(0), next(0) {}

FuncClass::FuncClass(): sockfd(-1), list(0), weight(0)
{
	buffer=new char[1024];
	buffer[0]=0;
}

FuncClass::~FuncClass()
{
	if (buffer) delete[] buffer;
}

const char* FuncClass::FuncName [14] =
													{ "getid" , "players" , "bunkbuy" ,
													"banksell" , "buyprice" , "sellprice" , "money" ,
													"raw" , "products" , "factories" , "isbdif" ,
													"issdif" , "getbdiff" , "getsdiff" };

const int FuncClass::FuncArgsNum [14] =
									{ 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 0 , 0 , 1 , 1 };

void FuncClass::servfd(int fd)
{
	sockfd=fd;
}

bool FuncClass::checkconnect()
{
	return (sockfd!=-1);
}

void FuncClass::waitnum()
{
	int i=0;

	botread();
	while (buffer[i]!='Y')
		++i;
	strcpy(buffer,buffer+i);
	weight-=i;
	mynum=find();
	write(sockfd,"market\r\n",8);
	buffer[weight=0]=0;
	botread();
	find();
	maxcol=find();
	Player *cur=new Player(1);
	list=cur;
	for (i=2;i<=maxcol;++i)
		cur=cur->next=new Player(i);
	editinfo();
}

void FuncClass::editinfo()
{
	Player *temp=list;
	char s[32];

	write(sockfd,"market\r\n",8);
	buffer[weight=0]=0;
	botread();
	find();find();
	buy=find();
	bprice=find();
	sell=find();
	sprice=find();
	for(int i=1;i<=maxcol;++i)
	{
		sprintf(s,"player %d\r\n",i);
		write(sockfd,s,strlen(s));
		buffer[weight=0]=0;
		botread();
		temp->money=find();
		temp->raw=find();
		temp->products=find();
		temp->factories=find();
		temp=temp->next;
	}
}

int FuncClass::find()
{
 	int i=0;
	char s[1024];

	while ((buffer[i]<'0')||(buffer[i]>'9')) { ++i; }
	if (i!=0)
	{
		memmove(buffer,buffer+i,weight-=i);
		i=0;
	}
	while ((buffer[i]>='0')&&(buffer[i]<='9'))
	{
		s[i]=buffer[i];
		++i;
	}
	s[i]=0;
	memmove(buffer,buffer+i,weight-=i);
	return strtoint(s);
}

void FuncClass::botread()
{
	do
	{
		Label:
		int k=read(sockfd,buffer+weight,1023-weight);
		if (!k)
		{
			win();
			exit(0);
		}
		weight+=k;
		buffer[weight]=0;
		if (weight<2) goto Label;
	} while (buffer[weight-2]!='>');
}

void FuncClass::fsell(int op1,int op2)
{
	char s[32];

	sprintf(s,"sell %d %d\r\n",op1,op2);
	write(sockfd,s,strlen(s));
	buffer[weight=0]=0;
	botread();
}

void FuncClass::fbuy(int op1,int op2)
{
	char s[32];

	sprintf(s,"buy %d %d\r\n",op1,op2);
	write(sockfd,s,strlen(s));
	buffer[weight=0]=0;
	botread();
}

void FuncClass::fprod(int op)
{
	char s[32];

	sprintf(s,"prod %d\r\n",op);
	write(sockfd,s,strlen(s));
	buffer[weight=0]=0;
	botread();
}

void FuncClass::fbuild()
{
	write(sockfd,"build\r\n",7);
	buffer[weight=0]=0;
	botread();
}

void FuncClass::fturn()
{
	write(sockfd,"turn\r\n",6);
	buffer[0]=0;
	botread();
	botread();
	checkdiff();
	editinfo();
}

void FuncClass::win()
{
	if (!strstr(buffer,"Month"))
	{
		if (strstr(buffer,"wins"))
			printf("\nI win this game\n\n");
		else
			printf("\nI lose this game\n\n");
	}
	buffer[0]=0;
	weight=0;
}

void FuncClass::checkdiff()
{
	char *str;
	int price,num;

	if (issdif())
	{
		while ((str=strstr(buffer,"sold")))
		{
			memmove(buffer,str,weight-=strlen(str));
			num=find();
			price=find();
			if (num!=mynum)
			{
				Player *temp=list;

				for (int i=1;i<num;++i)
					temp=temp->next;
				temp->sdiff=sell-price;
			}
		}
	}
	if (isbdif())
	{
		while ((str=strstr(buffer,"bought")))
		{
			memmove(buffer,str,weight-=strlen(str));
			num=find();
			price=find();
			if (num!=mynum)
			{
				Player *temp=list;

				for (int i=1;i<num;++i)
					temp=temp->next;
				temp->sdiff=price-buy;
			}
		}
	}
}

int FuncClass::getargnum(char *str) const
{
	int i=0;

	for (;i<14;++i)
		if (!strcmp(str,FuncName[i]))
			break;
	if (i==14)
	{
		char s[64];

		sprintf(s,"%s no such internal function",str);
		throw polizerr(s);
	}
	return FuncArgsNum[i];
}

int FuncClass::launch(char *name) const
{
	int i=0;

	for (;i<14;++i)
		if (!strcmp(name,FuncName[i]))
			break;
	switch (i)
	{
		case Getid:
			return getid();
		case Players:
			return players();
		case Bankbuy:
			return bankbuy();
		case Banksell:
			return banksell();
		case Buyprice:
			return buyprice();
		case Sellprice:
			return sellprice();
		case Isbdif:
			return isbdif();
		case Issdif:
			return issdif();
	}
	return 0;
}

int FuncClass::launch(char *name,int op) const
{
	int i=0;

	for (;i<14;++i)
		if (!strcmp(name,FuncName[i]))
			break;
	switch (i)
	{
		case Money:
			return money(op);
		case Raw:
			return raw(op);
		case Products:
			return products(op);
		case Factories:
			return factories(op);
		case Getbdiff:
			return getbdiff(op);
		case Getsdiff:
			return getsdiff(op);
	}
	return 0;
}

int FuncClass::getid() const
{
	return mynum;
}

int FuncClass::players() const
{
	return maxcol;
}

int FuncClass::banksell() const
{
	return buy;
}

int FuncClass::bankbuy() const
{
	return sell;
}

int FuncClass::sellprice() const
{
	return sprice;
}

int FuncClass::buyprice() const
{
	return bprice;
}

int FuncClass::money(int num) const
{
	Player *temp=list;

	for (int i=1;i<num;++i)
		temp=temp->next;
	return temp->money;
}

int FuncClass::raw(int num) const
{
	Player *temp=list;

	for (int i=1;i<num;++i)
		temp=temp->next;
	return temp->raw;
}

int FuncClass::products(int num) const
{
	Player *temp=list;

	for (int i=1;i<num;++i)
		temp=temp->next;
	return temp->products;
}

int FuncClass::factories(int num) const
{
	Player *temp=list;

	for (int i=1;i<num;++i)
		temp=temp->next;
	return temp->factories;
}

int FuncClass::isbdif() const
{
	Player *temp=list;
	int col=0;

	while (temp)
	{
		col+=temp->factories;
		temp=temp->next;
	}
	if (col>buy)
		return 1;
	else
		return 0;
}

int FuncClass::issdif() const
{
	Player *temp=list;
	int col=0;

	while (temp)
	{
		col+=temp->products;
		temp=temp->next;
	}
	if (col>sell)
		return 1;
	else
		return 0;
}

int FuncClass::getbdiff(int num) const
{
	Player *temp=list;

	for (int i=1;i<num;++i)
		temp=temp->next;
	return temp->bdiff;
}

int FuncClass::getsdiff(int num) const
{
	Player *temp=list;

	for (int i=1;i<num;++i)
		temp=temp->next;
	return temp->sdiff;
}
