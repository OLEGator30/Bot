#include <stdio.h>
#include <string.h>
#include "poliz.hpp"
#include "../errors/errors.hpp"

void PolizElem::Push(PolizItem **stack, PolizElem *elem)
{
	PolizItem *temp=*stack;

	if (!temp)
		*stack=new PolizItem(elem);
	else
	{
		while (temp->next)
			temp=temp->next;
		temp->next=new PolizItem(elem);
	}
}

PolizElem* PolizElem::Pop(PolizItem **stack)
{
	PolizItem *temp=*stack;
	PolizElem *elem;

	if (!temp)
		throw polizerr("stack is empty");
	if (!(temp->next))
	{
		elem=temp->elem;
		delete temp;
		*stack=0;
		return elem;
	}
	while (temp->next->next)
		temp=temp->next;
	elem=temp->next->elem;
	delete temp->next;
	temp->next=0;
	return elem;
}

void PolizConst::Evaluate(PolizItem **stack,PolizItem **curcmd) const
{
	Push(stack,Clone());
	*curcmd=(*curcmd)->next;
}

void PolizFunction::Evaluate(PolizItem **stack,PolizItem **curcmd) const
{
	PolizElem *res=EvaluateFun(stack);
	if (res) Push(stack, res);
	*curcmd=(*curcmd)->next;
}

PolizInt::PolizInt(int a): value(a) {}

PolizElem* PolizInt::Clone() const
{
	return new PolizInt(value);
}

int PolizInt::Get() const
{
	return value;
}

void PolizInt::print() const
{
	printf("%d (int)\n",value);
}

PolizString::PolizString(char* s)
{
	str=new char[strlen(s)+1];
	strcpy(str,s);
}

PolizElem* PolizString::Clone() const
{
	return new PolizString(str);
}

char* PolizString::Get() const
{
	return str;
}

PolizString::~PolizString()
{
	if (str) delete[] str;
}

void PolizString::print() const
{
	printf("%s (string)\n",str);
}

PolizVarAddr::PolizVarAddr(char* str,int i)
{
	item=table.foundvar(str);
	if (!(item->decl))
		throw polizerr("var not decl");
	for (;i>0;--i)
		item=item->nextidx;
}

varitem* PolizVarAddr::Get() const
{
	return item;
}

PolizElem* PolizVarAddr::Clone() const
{
	return new PolizVarAddr(item->name,0);
}

void PolizVarAddr::print() const
{
	printf("var addr\n");
}

PolizLabel::PolizLabel(char *str)
{
	if (str)
		item=table.foundlab(str);
	else
		item=0;
}

void PolizLabel::SetVal(PolizItem *poliz)
{
	item=new labitem(0);
	item->val=poliz;
}

PolizLabel::PolizLabel(PolizItem *poliz)
{
	item=new labitem(0);
	item->val=poliz;
}

PolizElem* PolizLabel::Clone() const
{
	if (item->name)
		return new PolizLabel(item->name);
	else
		return new PolizLabel(item->val);
}

PolizItem* PolizLabel::Get() const
{
	return item->val;
}

void PolizLabel::print() const
{
	printf("label\n");
}

void PolizOpGo::Evaluate(PolizItem **stack,PolizItem **curcmd) const
{
	PolizElem *operand=Pop(stack);
	PolizLabel *lab=dynamic_cast<PolizLabel*>(operand);
	if (!lab) throw polizerr("not a label");
	*curcmd=(lab->Get())->next;
	delete operand;
}

void PolizOpGo::print() const
{
	printf("!\n");
}

void PolizOpGoFalse::Evaluate(PolizItem **stack,PolizItem **curcmd) const
{
	PolizElem *operand1=Pop(stack);
	PolizLabel *lab=dynamic_cast<PolizLabel*>(operand1);
	if (!lab) throw polizerr("not a label");
	PolizElem *operand2=Pop(stack);
	PolizInt *exp=dynamic_cast<PolizInt*>(operand2);
	if (!exp) throw polizerr("not an int");
	if (!(exp->Get()))
		*curcmd=(lab->Get())->next;
	else
		*curcmd=(*curcmd)->next;
	delete operand1;
	delete operand2;
}

void PolizOpGoFalse::print() const
{
	printf("!F\n");
}

PolizElem* PolizVar::EvaluateFun(PolizItem **stack) const
{
	PolizElem *operand=Pop(stack);
	PolizVarAddr *i=dynamic_cast<PolizVarAddr*>(operand);
	if (!i) throw polizerr("not an varaddr");
	return new PolizInt((i->Get())->val);
}

void PolizVar::print() const
{
	printf("var\n");
}

PolizIdx::PolizIdx(bool i): set(i) {}

PolizElem* PolizIdx::EvaluateFun(PolizItem **stack) const
{
	PolizElem *operand1=Pop(stack);
	PolizInt *i1=dynamic_cast<PolizInt*>(operand1);
	if (!i1)
		throw polizerr("not an int");
	PolizElem *operand2=Pop(stack);
	PolizVarAddr *i2=dynamic_cast<PolizVarAddr*>(operand2);
	if (!i2)
		throw polizerr("not an addr");
	varitem *temp=i2->Get();
	int k,i=i1->Get();
	char *str=temp->name;
	k=i;
	if (set) --i;
	for (;i>0;--i)
	{
		varitem *t=temp;
		temp=temp->nextidx;
		if (!temp)
		{
			if (set)
			{
				temp=new varitem(0);
				t->nextidx=temp;
			}
			else
			{
				char s[64];
				if (k)
				{
					sprintf(s,"%s[%d] not declared",str,k);
					throw polizerr(s);
				}
				sprintf(s,"%s not declared",str);
				throw polizerr(s);
			}
		}
	}
	delete operand1;
	delete operand2;
	return new PolizVarAddr(str,k);
}

void PolizIdx::print() const
{
	printf("[]\n");
}

PolizElem* PolizFunPlus::EvaluateFun(PolizItem **stack) const
{
	PolizElem *operand1=Pop(stack);
	PolizInt *i1=dynamic_cast<PolizInt*>(operand1);
	if (!i1) throw polizerr("not an int");
	PolizElem *operand2=Pop(stack);
	PolizInt *i2=dynamic_cast<PolizInt*>(operand2);
	if (!i2) throw polizerr("not an int");
	int res=i2->Get()+i1->Get();
	delete operand1;
	delete operand2;
	return new PolizInt(res);
}

void PolizFunPlus::print() const
{
	printf("+\n");
}

PolizElem* PolizFunMinus::EvaluateFun(PolizItem **stack) const
{
	PolizElem *operand1=Pop(stack);
	PolizInt *i1=dynamic_cast<PolizInt*>(operand1);
	if (!i1) throw polizerr("not an int");
	PolizElem *operand2=Pop(stack);
	PolizInt *i2=dynamic_cast<PolizInt*>(operand2);
	if (!i2) throw polizerr("not an int");
	int res=i2->Get()-i1->Get();
	delete operand1;
	delete operand2;
	return new PolizInt(res);
}

void PolizFunMinus::print() const
{
	printf("-\n");
}

PolizElem* PolizFunDivision::EvaluateFun(PolizItem **stack) const
{
	PolizElem *operand1=Pop(stack);
	PolizInt *i1=dynamic_cast<PolizInt*>(operand1);
	if (!i1) throw polizerr("not an int");
	PolizElem *operand2=Pop(stack);
	PolizInt *i2=dynamic_cast<PolizInt*>(operand2);
	if (!i2) throw polizerr("not an int");
	int res=i2->Get()/i1->Get();
	delete operand1;
	delete operand2;
	return new PolizInt(res);
}

void PolizFunDivision::print() const
{
	printf("/\n");
}

PolizElem* PolizFunMultipl::EvaluateFun(PolizItem **stack) const
{
	PolizElem *operand1=Pop(stack);
	PolizInt *i1=dynamic_cast<PolizInt*>(operand1);
	if (!i1) throw polizerr("not an int");
	PolizElem *operand2=Pop(stack);
	PolizInt *i2=dynamic_cast<PolizInt*>(operand2);
	if (!i2) throw polizerr("not an int");
	int res=i2->Get()*i1->Get();
	delete operand1;
	delete operand2;
	return new PolizInt(res);
}

void PolizFunMultipl::print() const
{
	printf("*\n");
}

PolizElem* PolizFunPercent::EvaluateFun(PolizItem **stack) const
{
	PolizElem *operand1=Pop(stack);
	PolizInt *i1=dynamic_cast<PolizInt*>(operand1);
	if (!i1) throw polizerr("not an int");
	PolizElem *operand2=Pop(stack);
	PolizInt *i2=dynamic_cast<PolizInt*>(operand2);
	if (!i2) throw polizerr("not an int");
	int res=i2->Get()%i1->Get();
	delete operand1;
	delete operand2;
	return new PolizInt(res);
}

void PolizFunPercent::print() const
{
	printf("%%\n");
}

PolizElem* PolizFunAnd::EvaluateFun(PolizItem **stack) const
{
	PolizElem *operand1=Pop(stack);
	PolizInt *i1=dynamic_cast<PolizInt*>(operand1);
	if (!i1) throw polizerr("not an int");
	PolizElem *operand2=Pop(stack);
	PolizInt *i2=dynamic_cast<PolizInt*>(operand2);
	if (!i2) throw polizerr("not an int");
	int res=i2->Get()&&i1->Get();
	delete operand1;
	delete operand2;
	return new PolizInt(res);
}

void PolizFunAnd::print() const
{
	printf("&\n");
}

PolizElem* PolizFunOr::EvaluateFun(PolizItem **stack) const
{
	PolizElem *operand1=Pop(stack);
	PolizInt *i1=dynamic_cast<PolizInt*>(operand1);
	if (!i1) throw polizerr("not an int");
	PolizElem *operand2=Pop(stack);
	PolizInt *i2=dynamic_cast<PolizInt*>(operand2);
	if (!i2) throw polizerr("not an int");
	int res=i2->Get()||i1->Get();
	delete operand1;
	delete operand2;
	return new PolizInt(res);
}

void PolizFunOr::print() const
{
	printf("|\n");
}

PolizElem* PolizFunEqu::EvaluateFun(PolizItem **stack) const
{
	PolizElem *operand1=Pop(stack);
	PolizInt *i1=dynamic_cast<PolizInt*>(operand1);
	if (!i1) throw polizerr("not an int");
	PolizElem *operand2=Pop(stack);
	PolizInt *i2=dynamic_cast<PolizInt*>(operand2);
	if (!i2) throw polizerr("not an int");
	int res=i2->Get()==i1->Get();
	delete operand1;
	delete operand2;
	return new PolizInt(res);
}

void PolizFunEqu::print() const
{
	printf("==\n");
}

PolizElem* PolizFunMore::EvaluateFun(PolizItem **stack) const
{
	PolizElem *operand1=Pop(stack);
	PolizInt *i1=dynamic_cast<PolizInt*>(operand1);
	if (!i1) throw polizerr("not an int");
	PolizElem *operand2=Pop(stack);
	PolizInt *i2=dynamic_cast<PolizInt*>(operand2);
	if (!i2) throw polizerr("not an int");
	int res=i2->Get()>i1->Get();
	delete operand1;
	delete operand2;
	return new PolizInt(res);
}

void PolizFunMore::print() const
{
	printf(">\n");
}

PolizElem* PolizFunLess::EvaluateFun(PolizItem **stack) const
{
	PolizElem *operand1=Pop(stack);
	PolizInt *i1=dynamic_cast<PolizInt*>(operand1);
	if (!i1) throw polizerr("not an int");
	PolizElem *operand2=Pop(stack);
	PolizInt *i2=dynamic_cast<PolizInt*>(operand2);
	if (!i2) throw polizerr("not an int");
	int res=i2->Get()<i1->Get();
	delete operand1;
	delete operand2;
	return new PolizInt(res);
}

void PolizFunLess::print() const
{
	printf("<\n");
}

PolizElem* PolizFunAssig::EvaluateFun(PolizItem **stack) const
{
	PolizElem *operand1=Pop(stack);
	PolizInt *i1=dynamic_cast<PolizInt*>(operand1);
	if (!i1) throw polizerr("not an int");
	PolizElem *operand2=Pop(stack);
	PolizVarAddr *i2=dynamic_cast<PolizVarAddr*>(operand2);
	if (!i2) throw polizerr("not an addr");
	(i2->Get())->val=i1->Get();
	delete operand1;
	delete operand2;
	return 0;
}

void PolizFunAssig::print() const
{
	printf("=\n");
}

PolizElem* PolizFunNeg::EvaluateFun(PolizItem **stack) const
{
	PolizElem *operand=Pop(stack);
	PolizInt *i=dynamic_cast<PolizInt*>(operand);
	if (!i) throw polizerr("not an int");
	int res=!(i->Get());
	delete operand;
	return new PolizInt(res);
}

void PolizFunNeg::print() const
{
	printf("!\n");
}

void PolizPrint::Evaluate(PolizItem **stack,PolizItem **curcmd) const
{
	PolizElem *operand=Pop(stack);
	PolizInt *i=dynamic_cast<PolizInt*>(operand);
	if (!i)
	{
		PolizString *j=dynamic_cast<PolizString*>(operand);
		if (!j)
		{
			PolizPrintEnd *k=dynamic_cast<PolizPrintEnd*>(operand);
			if (!k) throw polizerr("WTF??");
			(*curcmd)=(*curcmd)->next;
		}
		else
		{
			(*this).Evaluate(stack,curcmd);
			printf("%s",j->Get());
		}
	}
	else
	{
		(*this).Evaluate(stack,curcmd);
		printf("%d",i->Get());
	}
}

void PolizPrint::print() const
{
	printf("print\n");
}

PolizElem* PolizPrintEnd::Clone() const
{
	return new PolizPrintEnd();
}

void PolizPrintEnd::print() const
{
	printf("print end\n");
}

PolizElem* PolizSell::EvaluateFun(PolizItem **stack) const
{
	return 0;
}

void PolizSell::print() const
{
	printf("sell\n");
}

PolizElem* PolizBuy::EvaluateFun(PolizItem **stack) const
{
	return 0;
}

void PolizBuy::print() const
{
	printf("buy\n");
}

PolizElem* PolizProd::EvaluateFun(PolizItem **stack) const
{
	return 0;
}

void PolizProd::print() const
{
	printf("prod\n");
}

PolizElem* PolizBuild::EvaluateFun(PolizItem **stack) const
{
	return 0;
}

void PolizBuild::print() const
{
	printf("build\n");
}

PolizElem* PolizJoin::EvaluateFun(PolizItem **stack) const
{
	return 0;
}

void PolizJoin::print() const
{
	printf("join\n");
}

PolizElem* PolizTurn::EvaluateFun(PolizItem **stack) const
{
	return 0;
}

void PolizTurn::print() const
{
	printf("turn\n");
}

PolizFunction0::PolizFunction0(char *str)
{
	name=new char[strlen(str)+1];
	strcpy(name,str);
}

PolizFunction0::~PolizFunction0()
{
	if (name) delete[] name;
}

PolizElem* PolizFunction0::EvaluateFun(PolizItem **stack) const
{
	return new PolizInt(table.internfunc(name));
}

void PolizFunction0::print() const
{
	printf("func()\n");
}

PolizFunction1::PolizFunction1(char *str)
{
	name=new char[strlen(str)+1];
	strcpy(name,str);
}

PolizFunction1::~PolizFunction1()
{
	if (name) delete[] name;
}

PolizElem* PolizFunction1::EvaluateFun(PolizItem **stack) const
{
	PolizElem *operand=Pop(stack);
	PolizInt *i=dynamic_cast<PolizInt*>(operand);
	if (!i) throw polizerr("not an int");
	delete operand;
	return new PolizInt(table.internfunc(name,i->Get()));
}

void PolizFunction1::print() const
{
	printf("func(exp)\n");
}
