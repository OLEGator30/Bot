#ifndef COMMON_HPP
#define COMMON_HPP

extern const char* StrKeyWords[9];

enum LexType { KeyWord, Function, Variable, Label, Number, String,
																	Equal, Operation, Bracket, Divider };

enum KeWords { LexIf, LexGoto, LexPrint, LexInt, LexBuy, LexSell, LexProd,
																								LexBuild, LexEndturn };

struct lexlist
{
	char* lexstr;
	int lexnum;
	LexType type;
	int line;
	lexlist* next;

	lexlist();
	void print();
	~lexlist();
};

int strtoint (char*);
const char* states2str(LexType);

#endif
