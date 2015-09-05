#include <assert.h>
#include <algorithm>
#include "defs.h"

int NN;
int CountBit(int x)
{
	int count=0;
	while(x)
	{
		if(x&1)	count++;
		reinterpret_cast<unsigned int &>(x)>>=1;
	}
	return count;
}
//------------
Suset::Suset(string alphabet[])
{
	//printf("suset:this %x\n",this);
	for(int i=0; i<NN; i++)
	{
		cc[i]=nc[i]=NN-1;
		n[i]=new sNum;
		n[i]->name=alphabet[i].c_str();
		n[i]->inset=this;

		//n[i]->printlink();
		
		n[i]->insetpos=i;
	}
	//printlink();
	//getchar();
}
Suset::Suset()	{assert(false);}
Suset::~Suset()
{
	for(int i=0; i<NN; i++)
		delete n[i];
}
Suset::Suset(const Suset &x)
{
	name=x.name;
	for(int i=0; i<NN; i++)
	{
		c[i]=x.c[i];
		n[i]=new sNum(*x.n[i]);
		n[i]->inset=this;
		cc[i]=x.cc[i];
		nc[i]=x.nc[i];
	}
}
void Suset::copy(Suset * ss)
{
	assert(name==ss->name);
	for(int i=0; i<NN; i++)
	{
		//c[i]->copy(ss->c[i]);// - отдельно при копировании cellset
		n[i]->copy(ss->n[i]);
		cc[i]=ss->cc[i];
		nc[i]=ss->nc[i];
	}
}
void Suset::sort()
{	//сортирует указатели на ÷еллы (по значени¤м этих указателей)
	std::copy(c,c+NN,cs);
	std::sort(cs,cs+NN);
}
//------------
sNum::sNum()	:nbit(NN)
{	x=0; for(int i=0, n=1; i<NN; i++,n<<=1)	x|=n;	}
void sNum::copy(sNum * pn)
{
	assert(name==pn->name);
	x=pn->x;
	nbit=pn->nbit;
}
sNum & sNum::disable(int i)
{
	//printf("sNum[%s].disable(%d)\n\tbefore: x=",name,i);printx(x);
	//printf(" nbit=%d\n",nbit);
	if(x&(1<<i))	nbit--;
	x&=~(1<<i);
	*inset->c[i] &= ~(1<<insetpos);
	//printf("\tafter: x=");printx(x);
	//printf(" nbit=%d",nbit);	getchar();
	assert(CountBit(x)==nbit);
	return *this;
}
sNum & sNum::operator=(int xx)
{
	for(int i=0, n=1; i<NN; i++, n<<=1)
		if(xx&n)	assert(x&n);
		else disable(i);
	return *this;
}
//------------
Cell::Cell()	:nbit(NN)
{
	x=0;
	for(int i=0, n=1; i<NN; i++,n<<=1)x|=n;	
	inset[0]=0;
	assert(CountBit(x)==nbit);
}
void Cell::copy(Cell * pc)
{
	assert(name==pc->name);
	x=pc->x;
	nbit=pc->nbit;
}
Cell & Cell::disable(int i)
{
	//printf("Cell[%s].disable(%d)\n\tbefore: x=",name,i);printx(x);
	//printf(" nbit=%d\n",nbit);
	if(x&(1<<i))	nbit--;
	x&=~(1<<i);
	for(int j=0; inset[j]; j++)
	{
		if(inset[j]->n[i]->x & (1<<insetpos[j]))
			inset[j]->n[i]->nbit--;
		inset[j]->n[i]->x &= ~(1<<insetpos[j]);
	}
	//printf("\tafter: x=");printx(x);
	//printf(" nbit=%d",nbit);	getchar();
	assert(CountBit(x)==nbit);
	return *this;
}
Cell & Cell::operator=(int xx)
{
	//printf("Cell= : ");printx(x);printf(" = ");printx(xx);
	for(int i=0, n=1; i<NN; i++, n<<=1)
		if(xx&n)	assert(x&n);
		else disable(i);
	//printf("\nafter : "); printx(x);	getchar();
	return *this;
}
//int main(){}
