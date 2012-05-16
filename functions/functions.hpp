#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

struct Player
{
	int number;
	int money;
	int raw;
	int products;
	int factories;
	int bdiff;
	int sdiff;
	Player *next;

	Player(int);
};

class FuncClass
{
	static const char* FuncName[14];
	static const int FuncArgsNum[14];

	int sockfd;

	Player *list;
	int mynum;
	char *buffer;
	int weight;
	int maxcol;
	int sell;
	int buy;
	int sprice;
	int bprice;

	void botread();
	int find();
	void editinfo();
	void win();
	void checkdiff();

	int getid() const;
	int players() const;
	int bankbuy() const;
	int banksell() const;
	int buyprice() const;
	int sellprice() const;
	int money(int) const;
	int raw(int) const;
	int products(int) const;
	int factories(int) const;

	public:

	FuncClass();
	~FuncClass();
	void servfd(int);
	bool checkconnect();
	void waitnum();

	void fsell(int,int);
	void fbuy(int,int);
	void fprod(int);
	void fbuild();
	void fturn();
	int isbdif() const;
	int issdif() const;
	int getbdiff(int) const;
	int getsdiff(int) const;

	int getargnum(char*) const;
	int launch(char*,int) const;
	int launch(char*) const;
	void newmonth();
};

extern FuncClass function;

#endif
