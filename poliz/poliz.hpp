#ifndef POLIZ_HPP
#define POLIZ_HPP

#include "../common/common.hpp"
#include "../functions/functions.hpp"

struct PolizItem;
struct labitem;
struct varitem;

class PolizElem
{
	public:

	virtual void Evaluate(PolizItem**,PolizItem**) const = 0;
	virtual ~PolizElem() {}
	static void Push(PolizItem**,PolizElem*);
	static PolizElem* Pop(PolizItem**);
	virtual void print() const = 0;
};

class PolizOpGo : public PolizElem
{
	public:

	PolizOpGo() {}
	virtual ~PolizOpGo() {}
	virtual void Evaluate(PolizItem**,PolizItem**) const;
	virtual void print() const;
};

class PolizOpGoFalse : public PolizElem
{
	public:

	PolizOpGoFalse() {}
	virtual ~PolizOpGoFalse() {}
	virtual void Evaluate(PolizItem**,PolizItem**) const;
	virtual void print() const;
};

class PolizConst : public PolizElem
{
	virtual PolizElem* Clone() const = 0;
	virtual void Evaluate(PolizItem**,PolizItem**) const;
};

class PolizFunction : public PolizElem
{
	virtual void Evaluate(PolizItem**,PolizItem**) const;
	virtual PolizElem* EvaluateFun(PolizItem**) const = 0;
};

class PolizInt : public PolizConst
{
	int value;

	public:

	PolizInt(int);
	virtual ~PolizInt() {}
	virtual PolizElem* Clone() const;
	int Get() const;
	virtual void print() const;
};

class PolizString : public PolizConst
{
	char *str;

	public:

	PolizString(char*);
	virtual ~PolizString();
	virtual PolizElem* Clone() const;
	char* Get() const;
	virtual void print() const;
};

class PolizVarAddr : public PolizConst
{
	varitem *item;

	public:

	PolizVarAddr(char*,int);
	virtual ~PolizVarAddr() {}
	virtual PolizElem* Clone() const;
	varitem* Get() const;
	virtual void print() const;
};

class PolizLabel : public PolizConst
{
	labitem *item;

	public:

	PolizLabel(char*);
	PolizLabel(PolizItem*);
	virtual ~PolizLabel() {}
	virtual PolizElem* Clone() const;
	PolizItem* Get() const;
	void SetVal(PolizItem*);
	virtual void print() const;
};

class PolizVar : public PolizFunction
{
	public:

	PolizVar() {}
	virtual ~PolizVar() {}
	PolizElem* EvaluateFun(PolizItem**) const;
	virtual void print() const;
};

class PolizIdx : public PolizFunction
{
	bool set;

	public:

	PolizIdx(bool);
	virtual ~PolizIdx() {}
	PolizElem* EvaluateFun(PolizItem**) const;
	virtual void print() const;
};

class PolizFunPlus : public PolizFunction
{
	public:

	PolizFunPlus() {}
	virtual ~PolizFunPlus() {}
	PolizElem* EvaluateFun(PolizItem**) const;
	virtual void print() const;
};

class PolizFunMinus : public PolizFunction
{
	public:

	PolizFunMinus() {}
	virtual ~PolizFunMinus() {}
	PolizElem* EvaluateFun(PolizItem**) const;
	virtual void print() const;
};

class PolizFunDivision : public PolizFunction
{
	public:

	PolizFunDivision() {}
	virtual ~PolizFunDivision() {}
	PolizElem* EvaluateFun(PolizItem**) const;
	virtual void print() const;
};

class PolizFunMultipl : public PolizFunction
{
	public:

	PolizFunMultipl() {}
	virtual ~PolizFunMultipl() {}
	PolizElem* EvaluateFun(PolizItem**) const;
	virtual void print() const;
};

class PolizFunPercent : public PolizFunction
{
	public:

	PolizFunPercent() {}
	virtual ~PolizFunPercent() {}
	PolizElem* EvaluateFun(PolizItem**) const;
	virtual void print() const;
};

class PolizFunAnd : public PolizFunction
{
	public:

	PolizFunAnd() {}
	virtual ~PolizFunAnd() {}
	PolizElem* EvaluateFun(PolizItem**) const;
	virtual void print() const;
};

class PolizFunOr : public PolizFunction
{
	public:

	PolizFunOr() {}
	virtual ~PolizFunOr() {}
	PolizElem* EvaluateFun(PolizItem**) const;
	virtual void print() const;
};

class PolizFunEqu : public PolizFunction
{
	public:

	PolizFunEqu() {}
	virtual ~PolizFunEqu() {}
	PolizElem* EvaluateFun(PolizItem**) const;
	virtual void print() const;
};

class PolizFunMore : public PolizFunction
{
	public:

	PolizFunMore() {}
	virtual ~PolizFunMore() {}
	PolizElem* EvaluateFun(PolizItem**) const;
	virtual void print() const;
};

class PolizFunLess : public PolizFunction
{
	public:

	PolizFunLess() {}
	virtual ~PolizFunLess() {}
	PolizElem* EvaluateFun(PolizItem**) const;
	virtual void print() const;
};

class PolizFunAssig : public PolizFunction
{
	public:

	PolizFunAssig() {}
	virtual ~PolizFunAssig() {}
	PolizElem* EvaluateFun(PolizItem**) const;
	virtual void print() const;
};

class PolizFunNeg : public PolizFunction
{
	public:

	PolizFunNeg() {}
	virtual ~PolizFunNeg() {}
	PolizElem* EvaluateFun(PolizItem**) const;
	virtual void print() const;
};

class PolizPrint : public PolizElem
{
	public:

	PolizPrint() {}
	virtual ~PolizPrint() {}
	virtual void print() const;
	void Evaluate(PolizItem**,PolizItem**) const;
};

class PolizPrintEnd : public PolizConst
{
	public:

	PolizPrintEnd() {}
	virtual ~PolizPrintEnd() {}
	virtual PolizElem* Clone() const;
	virtual void print() const;
};

class PolizSell : public PolizFunction
{
	public:

	PolizSell() {}
	virtual ~PolizSell() {}
	PolizElem* EvaluateFun(PolizItem**) const;
	virtual void print() const;
};

class PolizBuy : public PolizFunction
{
	public:

	PolizBuy() {}
	virtual ~PolizBuy() {}
	PolizElem* EvaluateFun(PolizItem**) const;
	virtual void print() const;
};

class PolizProd : public PolizFunction
{
	public:

	PolizProd() {}
	virtual ~PolizProd() {}
	PolizElem* EvaluateFun(PolizItem**) const;
	virtual void print() const;
};

class PolizJoin : public PolizFunction
{
	public:

	PolizJoin() {}
	virtual ~PolizJoin() {}
	PolizElem* EvaluateFun(PolizItem**) const;
	virtual void print() const;
};

class PolizBuild : public PolizFunction
{
	public:

	PolizBuild() {}
	virtual ~PolizBuild() {}
	PolizElem* EvaluateFun(PolizItem**) const;
	virtual void print() const;
};

class PolizTurn : public PolizFunction
{
	public:

	PolizTurn() {}
	virtual ~PolizTurn() {}
	PolizElem* EvaluateFun(PolizItem**) const;
	virtual void print() const;
};

class PolizFunction0 : public PolizFunction
{
	char *name;
	FuncClass table;

	public:

	PolizFunction0(char*);
	virtual ~PolizFunction0();
	PolizElem* EvaluateFun(PolizItem**) const;
	virtual void print() const;
};

class PolizFunction1 : public PolizFunction
{
	char *name;
	FuncClass table;

	public:

	PolizFunction1(char*);
	virtual ~PolizFunction1();
	PolizElem* EvaluateFun(PolizItem**) const;
	virtual void print() const;
};

#endif
