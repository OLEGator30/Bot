#include "poliz.hpp"
#include "../errors/errors.hpp"

int PolizItem::curnum=0;

PolizItem::PolizItem(PolizElem *e): elem(e), num(++curnum), next(0) {}

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

void PolizConst::Evaluate(PolizItem **stack,PolizItem *poliz,
																							PolizItem **curcmd) const
{
	Push(stack,Clone());
	*curcmd=(*curcmd)->next;
}

void PolizFunction::Evaluate(PolizItem **stack,PolizItem *poliz,
																							PolizItem **curcmd) const
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

PolizString::PolizString(char* s): str(s) {}

PolizElem* PolizString::Clone() const
{
	return new PolizString(str);
}

char* PolizString::Get() const
{
	return str;
}

PolizVarAddr::PolizVarAddr(char* str): item(table.getvar(str)) {}

int &PolizVarAddr::Get() const
{
	return item->val;
}

PolizElem* PolizVarAddr::Clone() const
{
	return new PolizVarAddr(item->name);
}

PolizLabel::PolizLabel(int a): value(a) {}

void PolizLabel::SetVal(int a)
{
	value=a;
}

PolizElem* PolizLabel::Clone() const
{
	return new PolizLabel(value);
}

int PolizLabel::Get() const
{
	return value;
}

void PolizOpGo::Evaluate(PolizItem **stack,PolizItem *poliz,
																							PolizItem **curcmd) const
{
	PolizElem *operand=Pop(stack);
	PolizLabel *lab=dynamic_cast<PolizLabel*>(operand);
	if (!lab) throw polizerr("not a label");
	PolizItem *addr=poliz;
	for (int num=lab->Get(); num>1; --num)
		addr=addr->next;
	*curcmd=addr;
	delete operand;
}

void PolizOpGoFalse::Evaluate(PolizItem **stack,PolizItem *poliz,
																							PolizItem **curcmd) const
{
	PolizElem *operand1=Pop(stack);
	PolizLabel *lab=dynamic_cast<PolizLabel*>(operand1);
	if (!lab) throw polizerr("not a label");
	PolizElem *operand2=Pop(stack);
	PolizInt *exp=dynamic_cast<PolizInt*>(operand2);
	if (!lab) throw polizerr("not a label");
	if (exp)
	{
		PolizItem *addr=poliz;
		for (int num=lab->Get(); num>1; --num)
			addr=addr->next;
		*curcmd=addr;
	}
	delete operand1;
	delete operand2;
}

PolizElem* PolizVar::EvaluateFun(PolizItem **stack) const
{
	PolizElem *operand=Pop(stack);
	PolizVarAddr *i=dynamic_cast<PolizVarAddr*>(operand);
	if (!i) throw polizerr("not an varaddr");
	return new PolizInt(i->Get());
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

PolizElem* PolizFunAssig::EvaluateFun(PolizItem **stack) const
{
	PolizElem *operand1=Pop(stack);
	PolizInt *i1=dynamic_cast<PolizInt*>(operand1);
	if (!i1) throw polizerr("not an int");
	PolizElem *operand2=Pop(stack);
	PolizVarAddr *i2=dynamic_cast<PolizVarAddr*>(operand2);
	if (!i2) throw polizerr("not an int");
	i2->Get()=i1->Get();
	delete operand1;
	delete operand2;
	return 0;
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

PolizElem* PolizPrint::EvaluateFun(PolizItem **stack) const
{
	return 0;
}

PolizElem* PolizSell::EvaluateFun(PolizItem **stack) const
{
	return 0;
}

PolizElem* PolizBuy::EvaluateFun(PolizItem **stack) const
{
	return 0;
}

PolizElem* PolizProd::EvaluateFun(PolizItem **stack) const
{
	return 0;
}

PolizElem* PolizBuild::EvaluateFun(PolizItem **stack) const
{
	return 0;
}

PolizElem* PolizTurn::EvaluateFun(PolizItem **stack) const
{
	return 0;
}