#include <assert.h>
#include <stdio.h>//при необходимости в интерфейсе определить printf() & getchar()

#include <algorithm>
#include <stack>
using std::stack;
#include <set>
using std::set;

#pragma warning (disable:4800)

#include "interface.h"

PR_level pr_level;
ST_level st_level;

MapC	cellset;
MapSs	suss;

void newCell(const char * name)
{
	MapC::iterator it=cellset.insert(make_pair(string(name),Cell())).first;
	it->second.name=it->first.c_str();
}
void newSuset(const char * name)
{
	MapSs::iterator it=suss.insert(make_pair(string(name),Suset(alphabet))).first;
	it->second.name=it->first.c_str();
}
void link(const char * susname, int n, const char * cellname)
{
	assert(suss.find(susname)!=suss.end());
	assert(cellset.find(cellname)!=cellset.end());
	Suset & s = suss[susname];
	Cell & c  = cellset[cellname];
	s.c[n]=&c;
	for(int i=0; i<99; i++)
		if(!c.inset[i])
		{
			c.inset[i]=&s;
			c.insetpos[i]=n;
			c.inset[i+1]=0;
			return;
		}
	assert(false);
}
void setc(const char * name, int x)
{
	assert(x<NN);
	assert(cellset.find(name)!=cellset.end());
	cellset[name]=1<<x;
}

namespace evalspace
{
	stack<Cell*> stc;//клетки, в которых осталась одна цифра, но которые не обработаны
	set<Suset*> chsss;//changed susets при вычеркивании
	void viro()
	{	//складывает в стек для обрабоки все клетки с одной цифрой (вырожденные клетки)
		if(pr_level&pr_titles)	printf("=== стартовая инициализация завершенных клеток ===\n");
		typedef map<string,Cell> mc;
		FOREACH(mc::iterator,it,cellset)
		{
			if(pr_level&pr_det1)	print(it->second);
			if(it->second.nbit==1)
			{	//складываем в стек всех кто завершен
				stc.push(&it->second);
				if(pr_level&pr_det1)	printf("push\n");
			}
		}
		if(st_level&st_titles)	getchar();
	}
	void gotoP()
	{	//обрабатывает все клетки, лежащие в стеке
		//т.е. вычеркивает цифру в такой клетке во всех множествах, в которых содержится эта клетка
		//если надо, новые клетки складываем в стек, а измененные множества отмечаем как измененные
		//во всех измененных сусетах ищем цифры, встречающиеся 1 раз, и кладем их в стек и начинаем сначала
	P:
		if(pr_level&pr_titles)	printf("===вычеркиваем готовых===\n");
		while(!stc.empty())
		{
			Suset * css;//current suset
			Cell * pc=stc.top();	stc.pop();//достали клетку
			if(pr_level&pr_det1)	printf("обрабатываем %s\n",pc->name);
			if(pc->nbit!=1)
				printf("error in gotoP:%s|",pc->name),printconf(*pc),
				printf("|\n"),getchar();
			assert(pc->nbit==1);
			for(int is=0; css=pc->inset[is]; is++)
			//по всем сусетам, в которых находится эта клетка
				for(int i=0; i<NN; i++)
				//по всем клеткам в каждом из них
					if(css->c[i]==pc)	continue;
					//кроме текущей клетки
					else
						if(*css->c[i] & *pc)
						{//если стоит вычеркивать
							Cell * cc=css->c[i];
							*cc &= ~*pc;//ВЫЧЕРКНУЛИ
							if(pr_level&pr_changes)	printf("вычеркнули из %s:|",cc->name)
								,printconf(*cc),printf("|\n");
							if(*cc == 0)	throw cc;//проверка на ошибки в условии
//???это мы проверили клетку, а проверить все sNum'ы - ???
// - а это на дальнейших этапах обработки выявится
							if(cc->nbit==1)
							{
								stc.push(cc);//кладем в стек новую очищенную клетку
								if(pr_level&pr_changes)	printf("готова %s\n",cc->name);
							}
							for(int is2=0; cc->inset[is2]; is2++)
							{
								if(pr_level&pr_det2 && chsss.find(cc->inset[is2])==chsss.end())
									printf("изменили множество %s\n",cc->inset[is2]->name);
								chsss.insert(cc->inset[is2]);
							}
						}
		}
		bool flag=false;
		if(pr_level&pr_titleres)	print(cellset);
		//printss(chsss);
		if(st_level&st_changes)	getchar();
		if(pr_level&pr_det2)
			FOREACH(set<Suset *>::iterator,it,chsss)
				printf("%s,",(*it)->name);
		if(pr_level&pr_det2)	printf(";\n");
		if(pr_level&st_changes)printf("=== ищем единственных ===\n");
		while(!chsss.empty())
		//по всем измененным сусетам
		{
			Suset * it=*chsss.begin();
			chsss.erase(chsss.begin());
			if(pr_level&pr_det1)	printf("проверяем %s\n",it->name);
			for(int i=0, n=1; i<NN; i++, n<<=1)
			//по всем цифрам
				if(it->n[i]->nbit ==1)
				//если эта цифра встречается в сусете 1 раз
				{
					if(pr_level&pr_det2)	printf("1 раз встретилась %sка\n",alphabet[i].c_str());
					for(int j=0; j<NN; j++)
					//ищем клетку, где находится эта цифра
						if(*it->c[j]&n)
						{
							if(pr_level&pr_det2)	printf("она находится в клетке %s\n",it->c[j]->name);
							if(it->c[j]->nbit!=1)
							{//и если там эта не одна
								flag=true;
								Cell * cc=it->c[j];
								*cc &= n;//ВЫЧЕРКНУЛИ
								if(pr_level&pr_det1 || pr_level&pr_det2)
									printf("вычеркнули остальное кроме %s в %s\n",alphabet[i].c_str(),cc->name);
								stc.push(cc);
								for(int is2=0; cc->inset[is2]; is2++)
								{
									if(pr_level&pr_det2 && chsss.find(cc->inset[is2])==chsss.end())
										printf("изменили множество %s\n",cc->inset[is2]->name);
									chsss.insert(cc->inset[is2]);
								}
								goto P;
							}
							else
							{
								if(pr_level&pr_det2)	printf("и эта клетка уже готова\n");
								break;
							}
						}
				}
		}
		if(pr_level&pr_titleres)	print(cellset);
		if(st_level&st_changes)		getchar();
		if(flag) goto P;
	}
	namespace U2space
	{
		//struct
		//{
			//int nbit;//сколько раз встретилась
			int masnum[NNmax];//цифра
			int masconf[NNmax];//ее конфигурация
		//}	mas[NNmax];//отсортирован по возраст. nbit
		int seet[NNmax+1], m[NNmax];//текущая конфигурация, номер цифры
		int N, //эквивалент локального n, размер-1 подбираемой конфигурации
			alp, //альфа, число претендентов
			maskconf;//маска обработанных клеток
		void printmas(int mask, int l, int num[], int conf[])
		{
			printf("\tmask:\t");	printconf(mask);
			for(int i=0; i<l; i++)
			{
				printf("\n\t%d\t",num[i]);
				printconf(conf[i]);
			}
			printf("\n");
		}
		bool f(int x)
		{
			if(x==-1)	return true;
			else if(x==N)	m[x]=alp-1;
			else m[x]=m[x+1]-1;
		met:
			if(m[x]<x)	return false;
			if(masconf[m[x]]&maskconf
				|| CountBit(seet[x]=seet[x+1] | masconf[m[x]])>N+1 
				|| !f(x-1))
			{
				m[x]--;
				goto met;
			}
			return true;
		}
	}
	template<typename Cell, typename sNum>
	void printU2title(int nl, int nr, Cell ** cs, int * cconf, sNum ** ns, bool chkstc, const char * name, const char * comment, bool stop)
	{
		printf("===U2(%s)%s===:[%d..%d):%d\n",name,comment,nl,nr,chkstc);
	}
	template<typename Cell, typename sNum>
	void printU2(int nl, int nr, Cell ** cs, int * cconf, sNum ** ns, bool chkstc, const char * name, const char * comment, bool stop)
	{
		printf("\tN:\t");
		for(int i=0; i<NN; i++)
			printf("%d",i);
			printf("\n");
		printf("\tnbit:\t");
		for(int i=0; i<NN; i++)
			printf("%d",cs[i]->nbit);
			printf("\n");
		printf("\tnconf:\t");
		for(int i=0; i<NN; i++)
			printf("%d",cconf[i]);
			printf("\n");
		printf("N:  name(nbit) +");printdot(NN);printf("+\n");
		for(int i=0, ni=1; i<NN; i++, ni<<=1)
		{
			for(int j=0, nj=1; j<NN; j++, nj<<=1)
				assert((bool)(*ns[i]&nj)==(bool)(*cs[j]&ni));
			printf("%d:%6s   (%d) |",i,ns[i]->name,ns[i]->nbit);
			printconf(*ns[i]);
			printf("|\t\t%d:%6s\n",i,cs[i]->name);
		}
		printf("\t       +");printdot(NN);printf("+\n");
		if(stop)	getchar();
	}
	template<typename Cell, typename sNum>
	bool U2(int nl, int nr, Cell ** cs, int * cconf, sNum ** ns, bool chkstc, const char * name)
	//в заданном диапозоне(nl,nr)
	//от конкретного сусета в одной из 2х конфигураций(c,cc,n)/(n,nc,c)
	{
		USENS U2space;
		if(pr_level&pr_titles2)
			printU2title(nl,nr,cs,cconf,ns,chkstc,name,"before",st_level>60);
		if(pr_level&pr_det1)
			printU2(nl,nr,cs,cconf,ns,chkstc,name,"before",st_level>60);
		alp=0;
		//инициализируем претендентов
		for(int n=0; n<nl; n++)
			for(int i=0; i<NN; i++)
				if(ns[i]->nbit==n+1)	
				{
					//mas[alp].nbit=n;
					masnum[alp]=i;
					masconf[alp]=*ns[i];
					alp++;
				}
		if(pr_level&pr_det2)
		{
			printf("after предынициализации mas\n");
			printmas(0,alp,masnum,masconf);
		}
		for(int n=nl; n<nr; n++)
		{
			//инициализируем претендентов
			for(int i=0; i<NN; i++)
				if(ns[i]->nbit==n+1)	
				{
					//mas[alp].nbit=n;
					masnum[alp]=i;
					masconf[alp]=*ns[i];
					alp++;
				}
			if(pr_level&pr_det2)
			{
				printf("after инициализации mas, n=%d\n",n);
				printmas(0,alp,masnum,masconf);
			}
			if(alp>=n+1)
			{
				N=n;	seet[n+1]=0;
				maskconf=0;
				//составляем маску из тех клеток, которые учавствовали в более мелких конфигурациях
				for(int i=0, k=1; i<NN; i++, k<<=1)
					if(cconf[i]<=n)	maskconf|=k;
				if(pr_level&pr_det2)
				{
					printf("before f(%d)\n",n);
					printmas(maskconf,alp,masnum,masconf);
				}
				while(f(n))//нашлась конфигурация
				{
					//помечаем эти клетки, что они вошли в конфигурацию размера n+1
					for(int i=0, k=1; i<NN; i++, k<<=1)
						if(seet[0]&k)	cconf[i]=n;
					int mask=0;//цифры, из которых состоит конфигурация
					for(int i=n; i!=-1; i--)
						mask |= 1<<masnum[m[i]];
					assert(CountBit(mask)==n+1);
					bool flag=false;
					for(int i=0, k=1; i<NN; i++, k<<=1)
						//по всем клеткам этой конфигурации, из которых можно что-то вычркнуть
						if(k&seet[0] && *cs[i]&~mask)
						{
							flag=true;
							*cs[i]&=mask;//ВЫЧЕРКНУЛИ
							if(pr_level&pr_changes)
								printf("ВЫЧЕРКНУЛИ из %s|",cs[i]->name)
								,printconf(*cs[i]),printf("|\n");
							if(chkstc && cs[i]->nbit==1)
								stc.push(reinterpret_cast< ::Cell*>(cs[i]));
						}
					if(flag)
					{
						if(!chkstc)
						{//проверка если наоборот
							for(int i=0, k=1; i<NN; i++, k<<=1)
								//по всем цифрам не входящим в конфигурацию
								if(k&mask)
									if(ns[i]->nbit==1)
										stc.push(reinterpret_cast< ::Cell*>(ns[i]));
									else if(ns[i]->nbit==0)
										throw reinterpret_cast< ::Cell*>(ns[i]);
							if(pr_level&pr_titleres2)	
								printU2(nl,nr,cs,cconf,ns,chkstc,name,"after",st_level&st_titles);
							//if(!stc.empty())		printf("pushed!!!"),getchar();
						}
						return true;
					}
					//вычисление маски к началу цикла
					for(int i=0, k=1; i<NN; i++, k<<=1)
						if(cconf[i]<=n)	maskconf|=k;
				}
			}
		}
		if(st_level&pr_titleres2)
			//printU2(nl,nr,cs,cconf,ns,chkstc,name,"after",st_level>50);
			printf("без изменений\n");
		return false;
	}
	namespace U1space
	{
		typedef Cell * Ostat[NNmax];//null terminated
		struct Peres//пересечение двух сусетов
		{
			mutable Ostat o[100];//terminated with s
				//клетки, принадлежащие каждому сусету из s и не принадлежащие данному пересечению
			Cell * c[NNmax];//null terminated
				//клетки входящие в пересечение
			mutable Suset * s[100];//null tetminated
				//сусеты, пересечение пар каждого из которых является этим пересечением
			Peres(){}
			Peres(const Peres & p)
			{
				int i;
				for(i=0; p.c[i]; i++)
					c[i]=p.c[i];
				c[i]=0;
				for(i=0; p.s[i]; i++)
				{
					s[i]=p.s[i];
					int j;
					for(j=0; p.o[i][j]; j++)
						o[i][j]=p.o[i][j];
					o[i][j]=0;
				}
				s[i]=0;
			}
			void print()const
			{
				for(const Cell * const* pc=c; *pc; pc++)
					printf("%s,",(*pc)->name);
				printf(";\n");
				for(int i=0; s[i]; i++)
				{
					printf("\t%s:",s[i]->name);
					for(int j=0; o[i][j]; j++)
						printf("%s,",o[i][j]->name);
					printf(";\n");
				}
			}
		private:
			Peres & operator=(const Peres &);
		};
		bool operator<(const Peres & l, const Peres & r)
		{//лексикографически по c
			for(int i=0; i<NN; i++)
				if(r.c[i]!=l.c[i])
					return l.c[i]==0 || l.c[i]<r.c[i];
				else 
					if(l.c[i]==0)
						return false;
			return false;
		}
		set<Peres> pereses;
	}
	void U1init()
	{
		USENS U1space;
		FOREACH(MapSs::iterator,it,suss)
			it->second.sort();
		FOREACH(MapC::iterator,it,cellset)
		{
			/*по всем парам сусетов, в которых содержится клетка *it
			ищем их пересечение
			проверяем есть ли оно в множестве пересечений
			ищем сусеты, входящие в это пересечение
				целл.сорт
			записываем его в сет
			*/
			for(Suset ** ss1=it->second.inset; ss1[1]; ss1++)
				for(Suset ** ss2=ss1+1; *ss2; ss2++)
				{//по всем парам сусетов
					Peres p;
					Cell ** endp;//предполагается, что не существует двух сусетов
					//совпадающих как множества
					(endp=std::set_intersection((*ss1)->cs,(*ss1)->cs+NN,
						(*ss2)->cs,(*ss2)->cs+NN,
						p.c))[0]=0;//ищем их пересечение
					if(endp-p.c<=1 || pereses.find(p)!=pereses.end())
						/*его размер мал или если оно уже создано*/	continue;
						//пересечения размером 1 обрабатываются в gotoP() и viro()
					if(pr_level&pr_det2)
					{
						printf("новое множество:");
						for(int i=0; p.c[i]; i++)
							printf("%s,",p.c[i]->name);
						printf(";\n");	
					}
					//if(st_level>55)	getchar();
					p.s[0]=*ss1;	p.s[1]=*ss2;	int k=2;
					for(Suset ** ss=it->second.inset; *ss; ss++)
						//пробегаем по всем возможным сусетам
						if(*ss==*ss1 || *ss==*ss2)	continue;
						else if(std::includes((*ss)->cs,(*ss)->cs+NN,p.c,endp))
							p.s[k++]=*ss;//если в ком-то из них содержится пересечение
							// - добавляем
					p.s[k]=0;
					pereses.insert(p);
				}
		}
		//создаем остатки
		FOREACH(set<Peres>::iterator,ip,pereses)
			for(int i=0; ip->s[i]; i++)
			{
				int ec; 
				for(ec=0; ip->c[ec]; ec++);
				std::set_difference(ip->s[i]->cs,ip->s[i]->cs+NN,ip->c,ip->c+ec,
				ip->o[i])[0]=0;
			}
		if(pr_level&pr_titles)	printf("=== U1init complete: ===\n");
		if(pr_level&pr_titles && pereses.empty())	
			printf("пересечений множеств размером больше одного не найдено\n");
		if(pr_level&pr_titleres)
			FOREACH(set<Peres>::iterator,ip,pereses)
				ip->print();
		if(st_level&st_titles)	getchar();
	}
	bool U1()
	{	//если цифра встречается в пересечении, и не встречается в одном из остатков
		//то ее нужно вычеркнуть из остальных остатков
		USENS U1space;
		/*
		or пересечения, и исключений, ...
		*/
		if(pr_level&pr_titles)	printf("=== U1 ===\n");
		bool flag=false;
		FOREACH(set<Peres>::iterator,ip,pereses)
		{
			int pm=0;//цифры, встречающиеся в пересечении
			for(int i=0; ip->c[i]; i++)
				pm|= *ip->c[i];
			for(int io=0; ip->s[io]; io++)
			{
				int om=0;//цифры, встречающиеся в остатке
				for(int i=0; ip->o[io][i]; i++)
					om|= *ip->o[io][i];
				if(int nm=pm&~om)//цифры, которые необходимо вычеркнуть
				{
					if(pr_level&pr_det1)	
					{
						printf("есть что вычеркнуть|");
						printconf(nm);
						printf("|\n");
					}
					if(pr_level&pr_u1)ip->print();
					for(int io2=0; ip->s[io2]; io2++)
						for(int ioc=0; ip->o[io2][ioc]; ioc++)
							if(*ip->o[io2][ioc]&nm)
							{
								Cell * cc=ip->o[io2][ioc];
								if(pr_level&pr_changes)	printf("вычеркиваю из %s\n",cc->name);
								flag=true;
								*cc&=~nm;
								if(cc->nbit==0)	throw cc;
								if(cc->nbit==1)	stc.push(cc);
							}
				}
			}
		}
		if(flag && pr_level&pr_titleres)
		if(flag && st_level&st_titles)	getchar();
		return flag;
	}
	bool cycl()
	{	//выполняет последвательно:
		//U2 для каждого сусета прямо и обратно
		//U1
		//U2 для каждого сусета прямо и обратно
		//возвращает наличие изменений
		bool f=false;
		typedef map<string,Suset> Mss;
		if(pr_level&pr_titles)			printf("=== цикл У2 прямой начало ===\n");
		FOREACH(Mss::iterator,it,suss)
			f=U2(0,NN/2+1,it->second.c,it->second.cc,it->second.n,true,it->second.name) ||f;
		if(f && pr_level&pr_titleres)	print(cellset);
		if(f && st_level&st_titles)		getchar();
		if(pr_level&pr_titles)			printf("=== цикл У2 обратный начало ===\n");
		FOREACH(Mss::iterator,it,suss)
			f=U2(0,NN/2+1,it->second.n,it->second.nc,it->second.c,false,it->second.name) ||f;
		if(f && pr_level&pr_titleres)	print(cellset);
		if(f && st_level&st_titles)		getchar();

		f=U1() ||f;

		if(pr_level&pr_titles)			printf("=== цикл У2 прямой завершение ===\n");
		FOREACH(Mss::iterator,it,suss)
			f=U2(NN/2+1,NN,it->second.c,it->second.cc,it->second.n,true,it->second.name) ||f;
		if(f && pr_level&pr_titleres)	print(cellset);
		if(f && st_level&st_titles)		getchar();
		if(pr_level&pr_titles)			printf("=== цикл У2 обратный завершение ===\n");
		FOREACH(Mss::iterator,it,suss)
			f=U2(NN/2+1,NN,it->second.n,it->second.nc,it->second.c,false,it->second.name) ||f;
		if(f && pr_level&pr_titleres)	print(cellset);
		if(f && st_level&st_titles)		getchar();
		//print();
		//printf("изменения : %d",f);
		//printf(" стек: %d",!stc.empty());
		//getchar();
		return f;
	}
};//namespace evalspace

int rezcount;//количество найденных решений
int glub;//текущая глубина предположений
bool evalrec()
{	//пытается решить, после всех попыток
	//берет одну клетку и начинает в ней делать предположения, и вызывать себя снова
	//возвращает, имеетя ли хоть одно решение
	USENS evalspace;
	if(pr_level&pr_perebor)		printf("глубина %d\n",glub);
	if(pr_level&pr_perebres)	print(cellset);
	//if(glub==28 || glub==29)		pr_level=pr_changes;
	//else							pr_level=pr_perebor;
	//getchar();
	try{
		if(!stc.empty())gotoP();//начальная обработка положенных в стек
			if(pr_level&pr_titles)	printf("после вырожденных(элементарных) вычислений\n");
			if(pr_level&pr_titleres)print(cellset);
			if(st_level&st_titles)	getchar();
		while(cycl())// || !stc.empty())
		{						//цикл
			if(!stc.empty())
				gotoP();
			if(pr_level&pr_titles)	printf("после очередной итерации вычислений\n");
			if(pr_level&pr_ss)		printss(suss);
			if(pr_level&pr_titleres)print(cellset);
			if(st_level&st_titles)	getchar();
		}
	}catch(Cell * c)
	{
		if(pr_level&pr_perebor)	printf("неудача: не осталось вариантов для клетки %s\n",c->name);
		if(pr_level&pr_perebres)print(cellset);
		if(st_level&st_perebor)	getchar();
		while(!stc.empty())	stc.pop();//ну нельзя у стека сделать clear
		chsss.clear();
		//pr_level=pr_det2;
		//st_level=st_titles;
		return false;
	}
			if(!stc.empty() || !chsss.empty())//проверка на наличие возможных ошибок
			{
				printf("грязно:(%d,%d)\n",stc.size(),chsss.size());
				//FOREACH(stack<Cell*>::iterator,it,stc)
				//	printf("%s,"(*it)->name);
				printf("%s\n",stc.top()->name);
				printf("\n");
				FOREACH(set<Suset*>::iterator,it,chsss)
					printf("%s",(*it)->name);
				print(cellset);
				getchar();
			}
	FOREACH(MapC::iterator,it,cellset)//а завершено ли решение
		if(it->second.nbit>1)
			goto perebor;//нет => перебор
	if(pr_level&pr_inout)	printf("нашлось %de решение\n",++rezcount);
	if(pr_level&pr_inout)	print(cellset);
	if(st_level&st_perebor)	getchar();
	return true;
perebor:
	if(pr_level&pr_perebres)printf("после вычислений\n");
	if(pr_level&pr_perebres)print(cellset);
	Cell * perecell;//клетка в которой что-то предполагаем
	for(int i=2; i<=NN; i++)//ищем клетку с наименьшей неопределенностью
		FOREACH(MapC::iterator,it,cellset)
			if(it->second.nbit==i)
			{
				perecell=&it->second;
				goto met2;
			}
	printf("error");//сюда никогда не попадем, хахаха, хотя -_-
		print(cellset);
	if(st_level)getchar();
	exit(2);
met2://нашли клетку по которой будем угадывать
	MapC cellset_tmp=cellset;	//перед тем как что-то предполагать надо ВСЁ сохранить 
	MapSs suss_tmp=suss;		//(при помощи конструкторов копирования по умолчанию)
	bool hasvariants=false;
	for(int irec=0, nrec=1; irec<NN; irec++, nrec<<=1)
	{//по всем цифрам
		//восстанавливаем контекст
		for(MapC::iterator it=cellset.begin(), ittmp=cellset_tmp.begin();
			it!=cellset.end() || ittmp!=cellset_tmp.end(); it++, ittmp++)
			it->second.copy(&ittmp->second);
		for(MapSs::iterator it=suss.begin(), ittmp=suss_tmp.begin();
			it!=suss.end() || ittmp!=suss_tmp.end(); it++, ittmp++)
			it->second.copy(&ittmp->second);
		//восстановили контекст
		//printf("%d:%6s|",glub,perecell->name);printconf(*perecell);printf("|perecell\n");
		//printf("%d:%6s|",glub,perecell->name);printconf(nrec);printf("|nrec\n");
		//getchar();
		if(*perecell&nrec)//если эту цифру мжно предположить
		{
			if(pr_level&pr_perebor)		printf("на глубине %d предположили в клетке %s вариант %s\n"
				,glub,perecell->name,alphabet[irec].c_str());
			if(st_level&st_perebor_)	getchar();
			*perecell&=nrec;//присваиваем эту цифру
			if(!stc.empty() || !chsss.empty())
			{
				printf("грязно:(%d,%d)\n",stc.size(),chsss.size());
				//FOREACH(stack<Cell*>::iterator,it,stc)
				//	printf("%s,"(*it)->name);
				printf("%s\n",stc.top()->name);
				printf("\n");
				FOREACH(set<Suset*>::iterator,it,chsss)
					printf("%s",(*it)->name);
				getchar();
			}
			stc.push(perecell);//и обработку нужно будет начать с нее
			//printss(suss);
			//printlink();
			//print();
			//getchar();
			glub++;
			hasvariants = evalrec()||hasvariants;//рекурсиясиясиясияисясиясиясиясиясиясияси
			glub--;
		}
	}
	return hasvariants;
}

void eval()
{	//запускает рекурсивное вычисление

	if(pr_level&pr_mainss)printss(suss);
	if(pr_level&pr_inout)printf("условие:\n");
	//printlink();
	if(pr_level&pr_inout)print(cellset);
	//if(st_level>=st_inout)getchar();

	USENS evalspace;
	U1init();
	rezcount=0;
	glub=0;
	viro();//сложить в стек все завершенные
	if(evalrec())
	{
		assert(rezcount!=0);
		if(pr_level&pr_eval)printf("найдено %d решений\n",rezcount);
		if(st_level&st_eval)getchar();
	}
	else
	{
		assert(rezcount==0);
		if(pr_level&pr_level)
			printf("исходное условие противоречиво, ни один вариант не подошел\n");
		if(st_level&st_level)getchar();
	}
}
