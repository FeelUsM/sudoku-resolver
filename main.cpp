#include <assert.h>
#include <stdio.h>

#include "interface.h"

string alphabet[NNmax];
int alphabetsize;

int printx(int c)
{	//выводит без пробелов все возможные цифры, содержащиеся в x
	//возвращает alphabetsize*количество выведенных цифр
	int p=0;
	for(int i=0, n=1; i<NN; i++, n<<=1)
		if(c&n)	printf("%s",alphabet[i].c_str())
			, p+=alphabetsize;
	return p;
}
/*extern*/
void print(const Cell & c)
{	//отладочный вывод: выводит: имя : x=содержимое nbit=%nbit и требует getchar()
	printf("%s : x=",c.name);printx((int)c);
	printf(" nbit=%d",c.nbit);	getchar();
}
void printcell(/*const*/MapC & cellset, const char * s, int l)
{	//выводит содержимое клетки в поле заданной длины (т.е. пробелы вконце дописывает)
	assert(cellset.find(s)!=cellset.end());
	Cell & c=cellset[s];//отвратительно, что не определен константный оператор [] у мэпа
	int p=printx(c);
	for(p; p<l; p++)	printf(" ");
}
namespace test_skbrackets
{
	class foo
	{
	public:
		int operator[](int i)		{	return 2;	}
		int operator[](int i)const	{	return 4;	}
	};
	void bar()
	{
		foo x;
		printf("%d ",x[2]);
		const foo y;
		printf("%d\n",y[1]);
	}
}
/*extern*/
int printdot(int n)
{	//печатают тире заданной длины
	for(int i=0; i<n; i++)
		printf("-");
	return n;
}
int printdot(int n, char * s)
{	//печатают тире заданной длины в строку
	for(int i=0; i<n; i++)
		s+=sprintf(s,"-");
	return NN;
}
/*extern*/
void print(/*const*/MapC & cellset)
{	//выводит все, что есть на данный ммент
	const char * cm[NNmax][NNmax]=//массив имен 9х9
	{
		{"lu_lu","lu_u","lu_ru","u_lu","u_u","u_ru","ru_lu","ru_u","ru_ru"},
		{"lu_l","lu_c","lu_r","u_l","u_c","u_r","ru_l","ru_c","ru_r"},
		{"lu_ld","lu_d","lu_rd","u_ld","u_d","u_rd","ru_ld","ru_d","ru_rd"},
		{"l_lu","l_u","l_ru","c_lu","c_u","c_ru","r_lu","r_u","r_ru"},
		{"l_l","l_c","l_r","c_l","c_c","c_r","r_l","r_c","r_r"},
		{"l_ld","l_d","l_rd","c_ld","c_d","c_rd","r_ld","r_d","r_rd"},
		{"ld_lu","ld_u","ld_ru","d_lu","d_u","d_ru","rd_lu","rd_u","rd_ru"},
		{"ld_l","ld_c","ld_r","d_l","d_c","d_r","rd_l","rd_c","rd_r"},
		{"ld_ld","ld_d","ld_rd","d_ld","d_d","d_rd","rd_ld","rd_d","rd_rd"}
	};
	int k[NNmax];//размеры столбцов для вывода
	for(int i=0; i<NN; i++)
	{
		k[i]=0;
		for(int j=0; j<NN; j++)
		{
			int tmp=cellset[cm[j][i]].nbit;//отвратительно, что не определен константный оператор [] у мэпа
			if(tmp>k[i])
				k[i]=tmp;
		}
	}
	//for(int i=0; i<NN; i++)
	//	printf("%d ",k[i]);
	printf("\n");
	printf("+");
	for(int ib=0; ib<3; ib++)
	{
		printdot(k[ib*3]+k[ib*3+1]+k[ib*3+2]+2);
		printf("+");
	}
	printf("\n");
	for(int lb=0; lb<3; lb++)
	{
		for(int lm=0; lm<3; lm++)
		{
			printf("|");
			for(int sb=0; sb<3; sb++)
			{
				printcell(cellset,cm[lb*3+lm][sb*3],k[sb*3]);
				printf(" ");
				printcell(cellset,cm[lb*3+lm][sb*3+1],k[sb*3+1]);
				printf(" ");
				printcell(cellset,cm[lb*3+lm][sb*3+2],k[sb*3+2]);
				printf("|");
			}
			printf("\n");
		}
	printf("+");
	for(int ib=0; ib<3; ib++)
	{
		printdot(k[ib*3]+k[ib*3+1]+k[ib*3+2]+2);
		printf("+");
	}
	printf("\n");
	}
}
/*extern*/
void printss(Suset * mss[])
{
	/*
	печатает состояние множеств по 3 на строку
	каждое множество в формате:
	---- имя множества ----
	N:																	 123456789
	nbit:																 кличество использований цифр
	nconf:																 размер конфигураций цифр
	                                                                    +---------+
	имя клетки(сколько цифр в ней встречается)размер конфигурации клетки|...v.v.v.|
	имя клетки(сколько цифр в ней встречается)размер конфигурации клетки|...v.v.v.|
	имя клетки(сколько цифр в ней встречается)размер конфигурации клетки|...v.v.v.|
	имя клетки(сколько цифр в ней встречается)размер конфигурации клетки|...v.v.v.|
	имя клетки(сколько цифр в ней встречается)размер конфигурации клетки|...v.v.v.|
	имя клетки(сколько цифр в ней встречается)размер конфигурации клетки|...v.v.v.|
	имя клетки(сколько цифр в ней встречается)размер конфигурации клетки|...v.v.v.|
	имя клетки(сколько цифр в ней встречается)размер конфигурации клетки|...v.v.v.|
	имя клетки(сколько цифр в ней встречается)размер конфигурации клетки|...v.v.v.|
	                                                                    +---------+
	*/
	if(!mss)	return;
	char scr[10][15][13+NNmax];
	int q=0;
	//int qmax=79/(12+NN);
	for(Suset **pss=mss; *pss; pss++)
	{
		Suset * ss=*pss;
		char * cs;//current string
		sprintf(scr[q][0],"---- %4s ----",ss->name);
	cs=scr[q][1];
	cs+=sprintf(cs,"N:         ");//11
		for(int i=0; i<NN; i++)	
			cs+=sprintf(cs,"%s",alphabet[i].c_str());
		sprintf(cs," ");
	cs=scr[q][2];
		cs+=sprintf(cs,"nbit:      ");
		for(int j=0; j<NN; j++)
			cs+=sprintf(cs,"%d",ss->n[j]->nbit);
		sprintf(cs," ");
	cs=scr[q][3];
		cs+=sprintf(cs,"nconf:     ");
		for(int j=0; j<NN; j++)
			cs+=sprintf(cs,"%d",ss->nc[j]);
		sprintf(cs," ");
	cs=scr[q][4];
		cs+=sprintf(cs,"        +");
		cs+=printdot(NN,cs);
		sprintf(cs,"+");
		//printf("%s",scr[q][4]);getchar();
	for(int i=0; i<NN; i++)
		{
			cs=scr[q][i+5];
			cs+=sprintf(cs,"%.6s(%d)%d|",ss->c[i]->name,ss->c[i]->nbit,ss->cc[i]);
			for(int j=0; j<NN; j++)
			{
				assert((bool)(*ss->c[i]&(1<<j)) == (bool)(*ss->n[j]&(1<<i)));
				if(*ss->c[i]&(1<<j))
					cs+=sprintf(cs,"v");
				else
					cs+=sprintf(cs,".");
				sprintf(cs,"|");
			}
		}
	cs=scr[q][14];
		cs+=sprintf(cs,"        +");
		cs+=printdot(NN,cs);
		sprintf(cs,"+");
		//scr[q][14][0]=0;
		q++;
		if(q==3)
		{
			for(int i=0; i<15; i++)
				printf("%25s%25s%25s\n",scr[0][i],scr[1][i],scr[2][i],scr[3][i]);
			//getchar();
			q=0;
		}
	}
	if(q)
	{
		for(q;q<3;q++)
			for(int i=0; i<15; i++)
				scr[q][i][0]=0;
		for(int i=0; i<15; i++)
			printf("%25s%25s%25s\n",scr[0][i],scr[1][i],scr[2][i]);
		//getchar();
		q=0;
	}
 	//getchar();
}
/*extern*/
void printss(Suset * s1, Suset *s2/*=0*/, Suset *s3/*=0*/)
{
	Suset * m[4];
	m[0]=s1;	m[1]=s2;	m[2]=s3;	m[3]=0;
	printss(m);
}
/*extern*/
/*
void printss(const set<Suset *>& sss)
{
	Suset ** m=new Suset* [sss.size()+1];
	int i=0;
	FOREACH(set<Suset*>::const_iterator,it,sss)
		m[i++]=*it;
	m[i]=0;
	printss(m);
}
*/
/*extern*/
void printconf(int x)
{
	for(int i=0, n=1; i<NN; i++, n<<=1)
		if(x&n)
			printf("v");
		else
			printf(".");
}
/*extern*/
void printss(MapSs & sss)
{
	Suset ** m=new Suset* [sss.size()+1];
	int i=0;
	FOREACH(MapSs::iterator,it,sss)
		m[i++]=&it->second;
	m[i]=0;
	printss(m);
}
//-----------------------------------------------------------------
void link9(const char * s, const char * c0, const char * c1, const char * c2, const char * c3, const char * c4, 
const char * c5, const char * c6, const char * c7, const char * c8)
{
	link(s,0,c0);
	link(s,1,c1);
	link(s,2,c2);
	link(s,3,c3);
	link(s,4,c4);
	link(s,5,c5);
	link(s,6,c6);
	link(s,7,c7);
	link(s,8,c8);
}
void create9x9base()
{
	newCell("lu_lu"	);	newCell("lu_u"	);	newCell("lu_ru"	);
	newCell("lu_l"	);	newCell("lu_c"	);	newCell("lu_r"	);
	newCell("lu_ld"	);	newCell("lu_d"	);	newCell("lu_rd"	);
	newCell("u_lu"	);	newCell("u_u"	);	newCell("u_ru"	);
	newCell("u_l"	);	newCell("u_c"	);	newCell("u_r"	);
	newCell("u_ld"	);	newCell("u_d"	);	newCell("u_rd"	);
	newCell("ru_lu"	);	newCell("ru_u"	);	newCell("ru_ru"	);
	newCell("ru_l"	);	newCell("ru_c"	);	newCell("ru_r"	);
	newCell("ru_ld"	);	newCell("ru_d"	);	newCell("ru_rd"	);
	newCell("l_lu"	);	newCell("l_u"	);	newCell("l_ru"	);
	newCell("l_l"	);	newCell("l_c"	);	newCell("l_r"	);
	newCell("l_ld"	);	newCell("l_d"	);	newCell("l_rd"	);
	newCell("c_lu"	);	newCell("c_u"	);	newCell("c_ru"	);
	newCell("c_l"	);	newCell("c_c"	);	newCell("c_r"	);
	newCell("c_ld"	);	newCell("c_d"	);	newCell("c_rd"	);
	newCell("r_lu"	);	newCell("r_u"	);	newCell("r_ru"	);
	newCell("r_l"	);	newCell("r_c"	);	newCell("r_r"	);
	newCell("r_ld"	);	newCell("r_d"	);	newCell("r_rd"	);
	newCell("ld_lu"	);	newCell("ld_u"	);	newCell("ld_ru"	);
	newCell("ld_l"	);	newCell("ld_c"	);	newCell("ld_r"	);
	newCell("ld_ld"	);	newCell("ld_d"	);	newCell("ld_rd"	);
	newCell("d_lu"	);	newCell("d_u"	);	newCell("d_ru"	);
	newCell("d_l"	);	newCell("d_c"	);	newCell("d_r"	);
	newCell("d_ld"	);	newCell("d_d"	);	newCell("d_rd"	);
	newCell("rd_lu"	);	newCell("rd_u"	);	newCell("rd_ru"	);
	newCell("rd_l"	);	newCell("rd_c"	);	newCell("rd_r"	);
	newCell("rd_ld"	);	newCell("rd_d"	);	newCell("rd_rd"	);

	newSuset("v1");	newSuset("h1");
	newSuset("v2");	newSuset("h2");
	newSuset("v3");	newSuset("h3");
	newSuset("v4");	newSuset("h4");
	newSuset("v5");	newSuset("h5");
	newSuset("v6");	newSuset("h6");
	newSuset("v7");	newSuset("h7");
	newSuset("v8");	newSuset("h8");
	newSuset("v9");	newSuset("h9");
		
	link9("v1","lu_lu","lu_l","lu_ld","l_lu","l_l","l_ld","ld_lu","ld_l","ld_ld");
	link9("v2","lu_u","lu_c","lu_d","l_u","l_c","l_d","ld_u","ld_c","ld_d");
	link9("v3","lu_ru","lu_r","lu_rd","l_ru","l_r","l_rd","ld_ru","ld_r","ld_rd");
	link9("v4","u_lu","u_l","u_ld","c_lu","c_l","c_ld","d_lu","d_l","d_ld");
	link9("v5","u_u","u_c","u_d","c_u","c_c","c_d","d_u","d_c","d_d");
	link9("v6","u_ru","u_r","u_rd","c_ru","c_r","c_rd","d_ru","d_r","d_rd");
	link9("v7","ru_lu","ru_l","ru_ld","r_lu","r_l","r_ld","rd_lu","rd_l","rd_ld");
	link9("v8","ru_u","ru_c","ru_d","r_u","r_c","r_d","rd_u","rd_c","rd_d");
	link9("v9","ru_ru","ru_r","ru_rd","r_ru","r_r","r_rd","rd_ru","rd_r","rd_rd");
	link9("h1","lu_lu","lu_u","lu_ru","u_lu","u_u","u_ru","ru_lu","ru_u","ru_ru");
	link9("h2","lu_l","lu_c","lu_r","u_l","u_c","u_r","ru_l","ru_c","ru_r");
	link9("h3","lu_ld","lu_d","lu_rd","u_ld","u_d","u_rd","ru_ld","ru_d","ru_rd");
	link9("h4","l_lu","l_u","l_ru","c_lu","c_u","c_ru","r_lu","r_u","r_ru");
	link9("h5","l_l","l_c","l_r","c_l","c_c","c_r","r_l","r_c","r_r");
	link9("h6","l_ld","l_d","l_rd","c_ld","c_d","c_rd","r_ld","r_d","r_rd");
	link9("h7","ld_lu","ld_u","ld_ru","d_lu","d_u","d_ru","rd_lu","rd_u","rd_ru");
	link9("h8","ld_l","ld_c","ld_r","d_l","d_c","d_r","rd_l","rd_c","rd_r");
	link9("h9","ld_ld","ld_d","ld_rd","d_ld","d_d","d_rd","rd_ld","rd_d","rd_rd");
}
void create9x9()
{
	create9x9base();

	newSuset("lu");	newSuset("u");	newSuset("ru");
	newSuset("l");	newSuset("c");	newSuset("r");
	newSuset("ld");	newSuset("d");	newSuset("rd");

	link9("lu","lu_lu","lu_u","lu_ru","lu_l","lu_c","lu_r","lu_ld","lu_d","lu_rd");
	link9("u","u_lu","u_u","u_ru","u_l","u_c","u_r","u_ld","u_d","u_rd");
	link9("ru","ru_lu","ru_u","ru_ru","ru_l","ru_c","ru_r","ru_ld","ru_d","ru_rd");
	link9("l","l_lu","l_u","l_ru","l_l","l_c","l_r","l_ld","l_d","l_rd");
	link9("c","c_lu","c_u","c_ru","c_l","c_c","c_r","c_ld","c_d","c_rd");
	link9("r","r_lu","r_u","r_ru","r_l","r_c","r_r","r_ld","r_d","r_rd");
	link9("ld","ld_lu","ld_u","ld_ru","ld_l","ld_c","ld_r","ld_ld","ld_d","ld_rd");
	link9("d","d_lu","d_u","d_ru","d_l","d_c","d_r","d_ld","d_d","d_rd");
	link9("rd","rd_lu","rd_u","rd_ru","rd_l","rd_c","rd_r","rd_ld","rd_d","rd_rd");
}
void create9x9figured()
{
	create9x9base();

	newSuset("s1");	
	link9("s1","lu_lu","lu_u","lu_l","lu_c","lu_ld","lu_d","l_lu","l_u","l_ru");
	newSuset("s2");	
	link9("s2","lu_ru","lu_r","u_lu","u_l","u_u","u_ru","ru_lu","ru_u","ru_ru");
	newSuset("s3");	
	link9("s3","lu_rd","u_ld","c_lu","u_c","u_d","c_u","u_r","u_rd","ru_l");
	newSuset("s4");	
	link9("s4","ru_c","ru_r","ru_ld","ru_d","ru_rd","r_lu","r_u","r_ru","r_c");
	newSuset("s5");	
	link9("s5","l_l","l_c","l_r","c_l","l_ld","l_d","ld_lu","ld_u","ld_c");
	newSuset("s6");	
	link9("s6","l_rd","c_ld","c_c","c_d","d_u","c_ru","c_r","c_rd","d_ru");
	newSuset("s7");	
	link9("s7","r_l","r_r","r_ld","r_d","r_rd","rd_u","rd_ru","rd_r","rd_rd");
	newSuset("s8");	
	link9("s8","ld_l","ld_ld","ld_d","ld_rd","ld_r","ld_ru","d_lu","d_l","d_ld");
	newSuset("s9");	
	link9("s9","d_c","d_d","d_r","d_rd","rd_lu","rd_l","rd_ld","rd_c","rd_d");
}
void create9x9with_diags()
{
	create9x9();
	newSuset("d1");	newSuset("d2");
	link9("d1","lu_lu","lu_c","lu_rd","c_lu","c_c","c_rd","rd_lu","rd_c","rd_rd");
	link9("d2","ld_ld","ld_c","ld_ru","c_ld","c_c","c_ru","ru_ld","ru_c","ru_ru");
}
void create9x9with_4quads()
{
	create9x9();
	newSuset("clu");	newSuset("cru");
	//link9("clu","","","","","","","","","");
	link9("clu","lu_c","lu_r","lu_d","lu_rd","c_lu","u_l","u_ld","l_u","l_ru");
	link9("cru","ru_c","ru_l","ru_d","ru_ld","c_ru","u_r","u_rd","r_u","r_lu");
	newSuset("cld");	newSuset("crd");
	link9("cld","ld_c","ld_u","ld_r","ld_ru","l_d","l_rd","d_l","d_lu","c_ld");
	link9("crd","rd_c","rd_u","rd_l","rd_lu","c_rd","r_d","r_ld","d_r","d_ru");
}
void mc(const char * name, int value)
{
	if(value)
		setc(name,value-1);
}
void set9x9(int m[9][9])
{
	mc("lu_lu"	,m[0][0]);	mc("lu_u"	,m[0][1]);	mc("lu_ru"	,m[0][2]);
	mc("lu_l"	,m[1][0]);	mc("lu_c"	,m[1][1]);	mc("lu_r"	,m[1][2]);
	mc("lu_ld"	,m[2][0]);	mc("lu_d"	,m[2][1]);	mc("lu_rd"	,m[2][2]);
	mc("u_lu"	,m[0][3]);	mc("u_u"	,m[0][4]);	mc("u_ru"	,m[0][5]);
	mc("u_l"	,m[1][3]);	mc("u_c"	,m[1][4]);	mc("u_r"	,m[1][5]);
	mc("u_ld"	,m[2][3]);	mc("u_d"	,m[2][4]);	mc("u_rd"	,m[2][5]);
	mc("ru_lu"	,m[0][6]);	mc("ru_u"	,m[0][7]);	mc("ru_ru"	,m[0][8]);
	mc("ru_l"	,m[1][6]);	mc("ru_c"	,m[1][7]);	mc("ru_r"	,m[1][8]);
	mc("ru_ld"	,m[2][6]);	mc("ru_d"	,m[2][7]);	mc("ru_rd"	,m[2][8]);
	mc("l_lu"	,m[3][0]);	mc("l_u"	,m[3][1]);	mc("l_ru"	,m[3][2]);
	mc("l_l"	,m[4][0]);	mc("l_c"	,m[4][1]);	mc("l_r"	,m[4][2]);
	mc("l_ld"	,m[5][0]);	mc("l_d"	,m[5][1]);	mc("l_rd"	,m[5][2]);
	mc("c_lu"	,m[3][3]);	mc("c_u"	,m[3][4]);	mc("c_ru"	,m[3][5]);
	mc("c_l"	,m[4][3]);	mc("c_c"	,m[4][4]);	mc("c_r"	,m[4][5]);
	mc("c_ld"	,m[5][3]);	mc("c_d"	,m[5][4]);	mc("c_rd"	,m[5][5]);
	mc("r_lu"	,m[3][6]);	mc("r_u"	,m[3][7]);	mc("r_ru"	,m[3][8]);
	mc("r_l"	,m[4][6]);	mc("r_c"	,m[4][7]);	mc("r_r"	,m[4][8]);
	mc("r_ld"	,m[5][6]);	mc("r_d"	,m[5][7]);	mc("r_rd"	,m[5][8]);
	mc("ld_lu"	,m[6][0]);	mc("ld_u"	,m[6][1]);	mc("ld_ru"	,m[6][2]);
	mc("ld_l"	,m[7][0]);	mc("ld_c"	,m[7][1]);	mc("ld_r"	,m[7][2]);
	mc("ld_ld"	,m[8][0]);	mc("ld_d"	,m[8][1]);	mc("ld_rd"	,m[8][2]);
	mc("d_lu"	,m[6][3]);	mc("d_u"	,m[6][4]);	mc("d_ru"	,m[6][5]);
	mc("d_l"	,m[7][3]);	mc("d_c"	,m[7][4]);	mc("d_r"	,m[7][5]);
	mc("d_ld"	,m[8][3]);	mc("d_d"	,m[8][4]);	mc("d_rd"	,m[8][5]);
	mc("rd_lu"	,m[6][6]);	mc("rd_u"	,m[6][7]);	mc("rd_ru"	,m[6][8]);
	mc("rd_l"	,m[7][6]);	mc("rd_c"	,m[7][7]);	mc("rd_r"	,m[7][8]);
	mc("rd_ld"	,m[8][6]);	mc("rd_d"	,m[8][7]);	mc("rd_rd"	,m[8][8]);
}

void create15x15() // "ромашка"
{
	/*
	    uul uu uur
	llu lu  u  ru  rru
	ll  l   c  r   rr
	lld ld  d  rd  rrd
	    ddl dd ddr
	*/
	newCell("lu_lu"	);	newCell("lu_u"	);	newCell("lu_ru"	);
	newCell("lu_l"	);	newCell("lu_c"	);	newCell("lu_r"	);
	newCell("lu_ld"	);	newCell("lu_d"	);	newCell("lu_rd"	);
	newCell("u_lu"	);	newCell("u_u"	);	newCell("u_ru"	);
	newCell("u_l"	);	newCell("u_c"	);	newCell("u_r"	);
	newCell("u_ld"	);	newCell("u_d"	);	newCell("u_rd"	);
	newCell("ru_lu"	);	newCell("ru_u"	);	newCell("ru_ru"	);
	newCell("ru_l"	);	newCell("ru_c"	);	newCell("ru_r"	);
	newCell("ru_ld"	);	newCell("ru_d"	);	newCell("ru_rd"	);
	newCell("l_lu"	);	newCell("l_u"	);	newCell("l_ru"	);
	newCell("l_l"	);	newCell("l_c"	);	newCell("l_r"	);
	newCell("l_ld"	);	newCell("l_d"	);	newCell("l_rd"	);
	newCell("c_lu"	);	newCell("c_u"	);	newCell("c_ru"	);
	newCell("c_l"	);	newCell("c_c"	);	newCell("c_r"	);
	newCell("c_ld"	);	newCell("c_d"	);	newCell("c_rd"	);
	newCell("r_lu"	);	newCell("r_u"	);	newCell("r_ru"	);
	newCell("r_l"	);	newCell("r_c"	);	newCell("r_r"	);
	newCell("r_ld"	);	newCell("r_d"	);	newCell("r_rd"	);
	newCell("ld_lu"	);	newCell("ld_u"	);	newCell("ld_ru"	);
	newCell("ld_l"	);	newCell("ld_c"	);	newCell("ld_r"	);
	newCell("ld_ld"	);	newCell("ld_d"	);	newCell("ld_rd"	);
	newCell("d_lu"	);	newCell("d_u"	);	newCell("d_ru"	);
	newCell("d_l"	);	newCell("d_c"	);	newCell("d_r"	);
	newCell("d_ld"	);	newCell("d_d"	);	newCell("d_rd"	);
	newCell("rd_lu"	);	newCell("rd_u"	);	newCell("rd_ru"	);
	newCell("rd_l"	);	newCell("rd_c"	);	newCell("rd_r"	);
	newCell("rd_ld"	);	newCell("rd_d"	);	newCell("rd_rd"	);

	newCell("llu_lu");	newCell("llu_u"	);	newCell("llu_ru");
	newCell("llu_l"	);	newCell("llu_c"	);	newCell("llu_r"	);
	newCell("llu_ld");	newCell("llu_d"	);	newCell("llu_rd");
	newCell("ll_lu" );	newCell("ll_u"	);	newCell("ll_ru");
	newCell("ll_l"	);	newCell("ll_c"	);	newCell("ll_r"	);
	newCell("ll_ld" );	newCell("ll_d"	);	newCell("ll_rd");
	newCell("lld_lu");	newCell("lld_u"	);	newCell("lld_ru");
	newCell("lld_l"	);	newCell("lld_c"	);	newCell("lld_r"	);
	newCell("lld_ld");	newCell("lld_d"	);	newCell("lld_rd");
	newCell("rru_lu");	newCell("rru_u"	);	newCell("rru_ru");
	newCell("rru_l"	);	newCell("rru_c"	);	newCell("rru_r"	);
	newCell("rru_ld");	newCell("rru_d"	);	newCell("rru_rd");
	newCell("rr_lu"	);	newCell("rr_u"	);	newCell("rr_ru"	);
	newCell("rr_l"	);	newCell("rr_c"	);	newCell("rr_r"	);
	newCell("rr_ld"	);	newCell("rr_d"	);	newCell("rr_rd"	);
	newCell("rrd_lu");	newCell("rrd_u"	);	newCell("rrd_ru");
	newCell("rrd_l"	);	newCell("rrd_c"	);	newCell("rrd_r"	);
	newCell("rrd_ld");	newCell("rrd_d"	);	newCell("rrd_rd");
	newCell("uul_lu");	newCell("uul_u"	);	newCell("uul_ru");
	newCell("uul_l"	);	newCell("uul_c"	);	newCell("uul_r"	);
	newCell("uul_ld");	newCell("uul_d"	);	newCell("uul_rd");
	newCell("uu_lu"	);	newCell("uu_u"	);	newCell("uu_ru"	);
	newCell("uu_l"	);	newCell("uu_c"	);	newCell("uu_r"	);
	newCell("uu_ld"	);	newCell("uu_d"	);	newCell("uu_rd"	);
	newCell("uur_lu");	newCell("uur_u"	);	newCell("uur_ru");
	newCell("uur_l"	);	newCell("uur_c"	);	newCell("uur_r"	);
	newCell("uur_ld");	newCell("uur_d"	);	newCell("uur_rd");
	newCell("ddl_lu");	newCell("ddl_u"	);	newCell("ddl_ru");
	newCell("ddl_l"	);	newCell("ddl_c"	);	newCell("ddl_r"	);
	newCell("ddl_ld");	newCell("ddl_d"	);	newCell("ddl_rd");
	newCell("dd_lu"	);	newCell("dd_u"	);	newCell("dd_ru"	);
	newCell("dd_l"	);	newCell("dd_c"	);	newCell("dd_r"	);
	newCell("dd_ld"	);	newCell("dd_d"	);	newCell("dd_rd"	);
	newCell("ddr_lu");	newCell("ddr_u"	);	newCell("ddr_ru");
	newCell("ddr_l"	);	newCell("ddr_c"	);	newCell("ddr_r"	);
	newCell("ddr_ld");	newCell("ddr_d"	);	newCell("ddr_rd");


	newSuset("lu");	newSuset("u");	newSuset("ru");
	newSuset("l");	newSuset("c");	newSuset("r");
	newSuset("ld");	newSuset("d");	newSuset("rd");
	newSuset("llu");newSuset("ll");	newSuset("lld");
	newSuset("rru");newSuset("rr");	newSuset("rrd");
	newSuset("uul");newSuset("uu");	newSuset("uur");
	newSuset("ddl");newSuset("dd");	newSuset("ddr");

	link9("lu","lu_lu","lu_u","lu_ru","lu_l","lu_c","lu_r","lu_ld","lu_d","lu_rd");
	link9("u" , "u_lu", "u_u", "u_ru", "u_l", "u_c", "u_r", "u_ld", "u_d", "u_rd");
	link9("ru","ru_lu","ru_u","ru_ru","ru_l","ru_c","ru_r","ru_ld","ru_d","ru_rd");
	link9("l" , "l_lu", "l_u", "l_ru", "l_l", "l_c", "l_r", "l_ld", "l_d", "l_rd");
	link9("c" , "c_lu", "c_u", "c_ru", "c_l", "c_c", "c_r", "c_ld", "c_d", "c_rd");
	link9("r" , "r_lu", "r_u", "r_ru", "r_l", "r_c", "r_r", "r_ld", "r_d", "r_rd");
	link9("ld","ld_lu","ld_u","ld_ru","ld_l","ld_c","ld_r","ld_ld","ld_d","ld_rd");
	link9("d" , "d_lu", "d_u", "d_ru", "d_l", "d_c", "d_r", "d_ld", "d_d", "d_rd");
	link9("rd","rd_lu","rd_u","rd_ru","rd_l","rd_c","rd_r","rd_ld","rd_d","rd_rd");

	link9("llu","llu_lu","llu_u","llu_ru","llu_l","llu_c","llu_r","llu_ld","llu_d","llu_rd");
	link9("ll" , "ll_lu", "ll_u", "ll_ru", "ll_l", "ll_c", "ll_r", "ll_ld", "ll_d", "ll_rd");
	link9("lld","lld_lu","lld_u","lld_ru","lld_l","lld_c","lld_r","lld_ld","lld_d","lld_rd");
	link9("rru","rru_lu","rru_u","rru_ru","rru_l","rru_c","rru_r","rru_ld","rru_d","rru_rd");
	link9("rr" , "rr_lu", "rr_u", "rr_ru", "rr_l", "rr_c", "rr_r", "rr_ld", "rr_d", "rr_rd");
	link9("rrd","rrd_lu","rrd_u","rrd_ru","rrd_l","rrd_c","rrd_r","rrd_ld","rrd_d","rrd_rd");
	link9("uul","uul_lu","uul_u","uul_ru","uul_l","uul_c","uul_r","uul_ld","uul_d","uul_rd");
	link9("uu" , "uu_lu", "uu_u", "uu_ru", "uu_l", "uu_c", "uu_r", "uu_ld", "uu_d", "uu_rd");
	link9("uur","uur_lu","uur_u","uur_ru","uur_l","uur_c","uur_r","uur_ld","uur_d","uur_rd");
	link9("ddl","ddl_lu","ddl_u","ddl_ru","ddl_l","ddl_c","ddl_r","ddl_ld","ddl_d","ddl_rd");
	link9("dd" , "dd_lu", "dd_u", "dd_ru", "dd_l", "dd_c", "dd_r", "dd_ld", "dd_d", "dd_rd");
	link9("ddr","ddr_lu","ddr_u","ddr_ru","ddr_l","ddr_c","ddr_r","ddr_ld","ddr_d","ddr_rd");

	newSuset("v01");
	newSuset("v02");
	newSuset("v03");
	newSuset("v04");	newSuset("uv04");	newSuset("dv04");
	newSuset("v05");	newSuset("uv05");	newSuset("dv05");
	newSuset("v06");	newSuset("uv06");	newSuset("dv06");
	newSuset("v07");	newSuset("uv07");	newSuset("dv07");
	newSuset("v08");	newSuset("uv08");	newSuset("dv08");
	newSuset("v09");	newSuset("uv09");	newSuset("dv09");
	newSuset("v10");	newSuset("uv10");	newSuset("dv10");
	newSuset("v11");	newSuset("uv11");	newSuset("dv11");
	newSuset("v12");	newSuset("uv12");	newSuset("dv12");
	newSuset("v13");
	newSuset("v14");
	newSuset("v15");
		
	link9("v01","llu_lu","llu_l","llu_ld","ll_lu","ll_l","ll_ld","lld_lu","lld_l","lld_ld");
	link9("v02","llu_u" ,"llu_c","llu_d" ,"ll_u" ,"ll_c","ll_d" ,"lld_u" ,"lld_c","lld_d" );
	link9("v03","llu_ru","llu_r","llu_rd","ll_ru","ll_r","ll_rd","lld_ru","lld_r","lld_rd");
	link9("v04","lu_lu","lu_l","lu_ld","l_lu","l_l","l_ld","ld_lu","ld_l","ld_ld");
	link9("v05","lu_u" ,"lu_c","lu_d" ,"l_u" ,"l_c","l_d" ,"ld_u" ,"ld_c","ld_d" );
	link9("v06","lu_ru","lu_r","lu_rd","l_ru","l_r","l_rd","ld_ru","ld_r","ld_rd");
	link9("v07", "u_lu", "u_l", "u_ld","c_lu","c_l","c_ld", "d_lu", "d_l", "d_ld");
	link9("v08", "u_u" , "u_c", "u_d" ,"c_u" ,"c_c","c_d" , "d_u" , "d_c", "d_d" );
	link9("v09", "u_ru", "u_r", "u_rd","c_ru","c_r","c_rd", "d_ru", "d_r", "d_rd");
	link9("v10","ru_lu","ru_l","ru_ld","r_lu","r_l","r_ld","rd_lu","rd_l","rd_ld");
	link9("v11","ru_u" ,"ru_c","ru_d" ,"r_u" ,"r_c","r_d" ,"rd_u" ,"rd_c","rd_d" );
	link9("v12","ru_ru","ru_r","ru_rd","r_ru","r_r","r_rd","rd_ru","rd_r","rd_rd");
	link9("v13","rru_lu","rru_l","rru_ld","rr_lu","rr_l","rr_ld","rrd_lu","rrd_l","rrd_ld");
	link9("v14","rru_u" ,"rru_c","rru_d" ,"rr_u" ,"rr_c","rr_d" ,"rrd_u" ,"rrd_c","rrd_d" );
	link9("v15","rru_ru","rru_r","rru_rd","rr_ru","rr_r","rr_rd","rrd_ru","rrd_r","rrd_rd");

	link9("uv04","lu_lu","lu_l","lu_ld","l_lu","l_l","l_ld","uul_lu","uul_l","uul_ld");
	link9("uv05","lu_u" ,"lu_c","lu_d" ,"l_u" ,"l_c","l_d" ,"uul_u" ,"uul_c","uul_d" );
	link9("uv06","lu_ru","lu_r","lu_rd","l_ru","l_r","l_rd","uul_ru","uul_r","uul_rd");
	link9("uv07", "u_lu", "u_l", "u_ld","c_lu","c_l","c_ld", "uu_lu", "uu_l", "uu_ld");
	link9("uv08", "u_u" , "u_c", "u_d" ,"c_u" ,"c_c","c_d" , "uu_u" , "uu_c", "uu_d" );
	link9("uv09", "u_ru", "u_r", "u_rd","c_ru","c_r","c_rd", "uu_ru", "uu_r", "uu_rd");
	link9("uv10","ru_lu","ru_l","ru_ld","r_lu","r_l","r_ld","uur_lu","uur_l","uur_ld");
	link9("uv11","ru_u" ,"ru_c","ru_d" ,"r_u" ,"r_c","r_d" ,"uur_u" ,"uur_c","uur_d" );
	link9("uv12","ru_ru","ru_r","ru_rd","r_ru","r_r","r_rd","uur_ru","uur_r","uur_rd");

	link9("dv04","ddl_lu","ddl_l","ddl_ld","l_lu","l_l","l_ld","ld_lu","ld_l","ld_ld");
	link9("dv05","ddl_u" ,"ddl_c","ddl_d" ,"l_u" ,"l_c","l_d" ,"ld_u" ,"ld_c","ld_d" );
	link9("dv06","ddl_ru","ddl_r","ddl_rd","l_ru","l_r","l_rd","ld_ru","ld_r","ld_rd");
	link9("dv07", "dd_lu", "dd_l", "dd_ld","c_lu","c_l","c_ld", "d_lu", "d_l", "d_ld");
	link9("dv08", "dd_u" , "dd_c", "dd_d" ,"c_u" ,"c_c","c_d" , "d_u" , "d_c", "d_d" );
	link9("dv09", "dd_ru", "dd_r", "dd_rd","c_ru","c_r","c_rd", "d_ru", "d_r", "d_rd");
	link9("dv10","ddr_lu","ddr_l","ddr_ld","r_lu","r_l","r_ld","rd_lu","rd_l","rd_ld");
	link9("dv11","ddr_u" ,"ddr_c","ddr_d" ,"r_u" ,"r_c","r_d" ,"rd_u" ,"rd_c","rd_d" );
	link9("dv12","ddr_ru","ddr_r","ddr_rd","r_ru","r_r","r_rd","rd_ru","rd_r","rd_rd");

	newSuset("h01");
	newSuset("h02");
	newSuset("h03");
	newSuset("h04");	newSuset("lh04");	newSuset("rh04");
	newSuset("h05");	newSuset("lh05");	newSuset("rh05");
	newSuset("h06");	newSuset("lh06");	newSuset("rh06");
	newSuset("h07");	newSuset("lh07");	newSuset("rh07");
	newSuset("h08");	newSuset("lh08");	newSuset("rh08");
	newSuset("h09");	newSuset("lh09");	newSuset("rh09");
	newSuset("h10");	newSuset("lh10");	newSuset("rh10");
	newSuset("h11");	newSuset("lh11");	newSuset("rh11");
	newSuset("h12");	newSuset("lh12");	newSuset("rh12");
	newSuset("h13");
	newSuset("h14");
	newSuset("h15");

	link9("h01","uul_lu","uul_u","uul_ru","uu_lu","uu_u","uu_ru","uur_lu","uur_u","uur_ru");
	link9("h02","uul_l" ,"uul_c","uul_r" ,"uu_l" ,"uu_c","uu_r" ,"uur_l" ,"uur_c","uur_r" );
	link9("h03","uul_ld","uul_d","uul_rd","uu_ld","uu_d","uu_rd","uur_ld","uur_d","uur_rd");
	link9("h04","lu_lu","lu_u","lu_ru","u_lu","u_u","u_ru","ru_lu","ru_u","ru_ru");
	link9("h05","lu_l" ,"lu_c","lu_r" ,"u_l" ,"u_c","u_r" ,"ru_l" ,"ru_c","ru_r" );
	link9("h06","lu_ld","lu_d","lu_rd","u_ld","u_d","u_rd","ru_ld","ru_d","ru_rd");
	link9("h07", "l_lu", "l_u", "l_ru","c_lu","c_u","c_ru", "r_lu", "r_u", "r_ru");
	link9("h08", "l_l" , "l_c", "l_r" ,"c_l" ,"c_c","c_r" , "r_l" , "r_c", "r_r" );
	link9("h09", "l_ld", "l_d", "l_rd","c_ld","c_d","c_rd", "r_ld", "r_d", "r_rd");
	link9("h10","ld_lu","ld_u","ld_ru","d_lu","d_u","d_ru","rd_lu","rd_u","rd_ru");
	link9("h11","ld_l" ,"ld_c","ld_r" ,"d_l" ,"d_c","d_r" ,"rd_l" ,"rd_c","rd_r" );
	link9("h12","ld_ld","ld_d","ld_rd","d_ld","d_d","d_rd","rd_ld","rd_d","rd_rd");
	link9("h13","ddl_lu","ddl_u","ddl_ru","dd_lu","dd_u","dd_ru","ddr_lu","ddr_u","ddr_ru");
	link9("h14","ddl_l" ,"ddl_c","ddl_r" ,"dd_l" ,"dd_c","dd_r" ,"ddr_l" ,"ddr_c","ddr_r" );
	link9("h15","ddl_ld","ddl_d","ddl_rd","dd_ld","dd_d","dd_rd","ddr_ld","ddr_d","ddr_rd");

	link9("lh04","lu_lu","lu_u","lu_ru","u_lu","u_u","u_ru","llu_lu","llu_u","llu_ru");
	link9("lh05","lu_l" ,"lu_c","lu_r" ,"u_l" ,"u_c","u_r" ,"llu_l" ,"llu_c","llu_r" );
	link9("lh06","lu_ld","lu_d","lu_rd","u_ld","u_d","u_rd","llu_ld","llu_d","llu_rd");
	link9("lh07", "l_lu", "l_u", "l_ru","c_lu","c_u","c_ru", "ll_lu", "ll_u", "ll_ru");
	link9("lh08", "l_l" , "l_c", "l_r" ,"c_l" ,"c_c","c_r" , "ll_l" , "ll_c", "ll_r" );
	link9("lh09", "l_ld", "l_d", "l_rd","c_ld","c_d","c_rd", "ll_ld", "ll_d", "ll_rd");
	link9("lh10","ld_lu","ld_u","ld_ru","d_lu","d_u","d_ru","lld_lu","lld_u","lld_ru");
	link9("lh11","ld_l" ,"ld_c","ld_r" ,"d_l" ,"d_c","d_r" ,"lld_l" ,"lld_c","lld_r" );
	link9("lh12","ld_ld","ld_d","ld_rd","d_ld","d_d","d_rd","lld_ld","lld_d","lld_rd");

	link9("rh04","rru_lu","rru_u","rru_ru","u_lu","u_u","u_ru","ru_lu","ru_u","ru_ru");
	link9("rh05","rru_l" ,"rru_c","rru_r" ,"u_l" ,"u_c","u_r" ,"ru_l" ,"ru_c","ru_r" );
	link9("rh06","rru_ld","rru_d","rru_rd","u_ld","u_d","u_rd","ru_ld","ru_d","ru_rd");
	link9("rh07", "rr_lu", "rr_u", "rr_ru","c_lu","c_u","c_ru", "r_lu", "r_u", "r_ru");
	link9("rh08", "rr_l" , "rr_c", "rr_r" ,"c_l" ,"c_c","c_r" , "r_l" , "r_c", "r_r" );
	link9("rh09", "rr_ld", "rr_d", "rr_rd","c_ld","c_d","c_rd", "r_ld", "r_d", "r_rd");
	link9("rh10","rrd_lu","rrd_u","rrd_ru","d_lu","d_u","d_ru","rd_lu","rd_u","rd_ru");
	link9("rh11","rrd_l" ,"rrd_c","rrd_r" ,"d_l" ,"d_c","d_r" ,"rd_l" ,"rd_c","rd_r" );
	link9("rh12","rrd_ld","rrd_d","rrd_rd","d_ld","d_d","d_rd","rd_ld","rd_d","rd_rd");

}
void set15x15(int m[15][15])
{
	/*
	    uul uu uur
	llu lu  u  ru  rru
	ll  l   c  r   rr
	lld ld  d  rd  rrd
	    ddl dd ddr
	*/
	mc("llu_lu"	,m[ 3][0]);	mc("llu_u"	,m[ 3][1]);	mc("llu_ru"	,m[ 3][2]);
	mc("llu_l"	,m[ 4][0]);	mc("llu_c"	,m[ 4][1]);	mc("llu_r"	,m[ 4][2]);
	mc("llu_ld"	,m[ 5][0]);	mc("llu_d"	,m[ 5][1]);	mc("llu_rd"	,m[ 5][2]);
	mc( "ll_lu"	,m[ 6][0]);	mc( "ll_u"	,m[ 6][1]);	mc( "ll_ru"	,m[ 6][2]);
	mc( "ll_l"	,m[ 7][0]);	mc( "ll_c"	,m[ 7][1]);	mc( "ll_r"	,m[ 7][2]);
	mc( "ll_ld"	,m[ 8][0]);	mc( "ll_d"	,m[ 8][1]);	mc( "ll_rd"	,m[ 8][2]);
	mc("lld_lu"	,m[ 9][0]);	mc("lld_u"	,m[ 9][1]);	mc("lld_ru"	,m[ 9][2]);
	mc("lld_l"	,m[10][0]);	mc("lld_c"	,m[10][1]);	mc("lld_r"	,m[10][2]);
	mc("lld_ld"	,m[11][0]);	mc("lld_d"	,m[11][1]);	mc("lld_rd"	,m[11][2]);

	mc("uul_lu"	,m[ 0][3]);	mc("uul_u"	,m[ 0][4]);	mc("uul_ru"	,m[ 0][5]);
	mc("uul_l"	,m[ 1][3]);	mc("uul_c"	,m[ 1][4]);	mc("uul_r"	,m[ 1][5]);
	mc("uul_ld"	,m[ 2][3]);	mc("uul_d"	,m[ 2][4]);	mc("uul_rd"	,m[ 2][5]);
	mc("lu_lu"	,m[ 3][3]);	mc("lu_u"	,m[ 3][4]);	mc("lu_ru"	,m[ 3][5]);
	mc("lu_l"	,m[ 4][3]);	mc("lu_c"	,m[ 4][4]);	mc("lu_r"	,m[ 4][5]);
	mc("lu_ld"	,m[ 5][3]);	mc("lu_d"	,m[ 5][4]);	mc("lu_rd"	,m[ 5][5]);
	mc( "l_lu"	,m[ 6][3]);	mc( "l_u"	,m[ 6][4]);	mc( "l_ru"	,m[ 6][5]);
	mc( "l_l"	,m[ 7][3]);	mc( "l_c"	,m[ 7][4]);	mc( "l_r"	,m[ 7][5]);
	mc( "l_ld"	,m[ 8][3]);	mc( "l_d"	,m[ 8][4]);	mc( "l_rd"	,m[ 8][5]);
	mc("ld_lu"	,m[ 9][3]);	mc("ld_u"	,m[ 9][4]);	mc("ld_ru"	,m[ 9][5]);
	mc("ld_l"	,m[10][3]);	mc("ld_c"	,m[10][4]);	mc("ld_r"	,m[10][5]);
	mc("ld_ld"	,m[11][3]);	mc("ld_d"	,m[11][4]);	mc("ld_rd"	,m[11][5]);
	mc("ddl_lu"	,m[12][3]);	mc("ddl_u"	,m[12][4]);	mc("ddl_ru"	,m[12][5]);
	mc("ddl_l"	,m[13][3]);	mc("ddl_c"	,m[13][4]);	mc("ddl_r"	,m[13][5]);
	mc("ddl_ld"	,m[14][3]);	mc("ddl_d"	,m[14][4]);	mc("ddl_rd"	,m[14][5]);

	mc("uu_lu"	,m[ 0][6]);	mc("uu_u"	,m[ 0][7]);	mc("uu_ru"	,m[ 0][8]);
	mc("uu_l"	,m[ 1][6]);	mc("uu_c"	,m[ 1][7]);	mc("uu_r"	,m[ 1][8]);
	mc("uu_ld"	,m[ 2][6]);	mc("uu_d"	,m[ 2][7]);	mc("uu_rd"	,m[ 2][8]);
	mc("u_lu"	,m[ 3][6]);	mc("u_u"	,m[ 3][7]);	mc("u_ru"	,m[ 3][8]);
	mc("u_l"	,m[ 4][6]);	mc("u_c"	,m[ 4][7]);	mc("u_r"	,m[ 4][8]);
	mc("u_ld"	,m[ 5][6]);	mc("u_d"	,m[ 5][7]);	mc("u_rd"	,m[ 5][8]);
	mc("c_lu"	,m[ 6][6]);	mc("c_u"	,m[ 6][7]);	mc("c_ru"	,m[ 6][8]);
	mc("c_l"	,m[ 7][6]);	mc("c_c"	,m[ 7][7]);	mc("c_r"	,m[ 7][8]);
	mc("c_ld"	,m[ 8][6]);	mc("c_d"	,m[ 8][7]);	mc("c_rd"	,m[ 8][8]);
	mc("d_lu"	,m[ 9][6]);	mc("d_u"	,m[ 9][7]);	mc("d_ru"	,m[ 9][8]);
	mc("d_l"	,m[10][6]);	mc("d_c"	,m[10][7]);	mc("d_r"	,m[10][8]);
	mc("d_ld"	,m[11][6]);	mc("d_d"	,m[11][7]);	mc("d_rd"	,m[11][8]);
	mc("dd_lu"	,m[12][6]);	mc("dd_u"	,m[12][7]);	mc("dd_ru"	,m[12][8]);
	mc("dd_l"	,m[13][6]);	mc("dd_c"	,m[13][7]);	mc("dd_r"	,m[13][8]);
	mc("dd_ld"	,m[14][6]);	mc("dd_d"	,m[14][7]);	mc("dd_rd"	,m[14][8]);

	mc("uur_lu"	,m[ 0][9]);	mc("uur_u"	,m[ 0][10]);	mc("uur_ru"	,m[ 0][11]);
	mc("uur_l"	,m[ 1][9]);	mc("uur_c"	,m[ 1][10]);	mc("uur_r"	,m[ 1][11]);
	mc("uur_ld"	,m[ 2][9]);	mc("uur_d"	,m[ 2][10]);	mc("uur_rd"	,m[ 2][11]);
	mc("ru_lu"	,m[ 3][9]);	mc("ru_u"	,m[ 3][10]);	mc("ru_ru"	,m[ 3][11]);
	mc("ru_l"	,m[ 4][9]);	mc("ru_c"	,m[ 4][10]);	mc("ru_r"	,m[ 4][11]);
	mc("ru_ld"	,m[ 5][9]);	mc("ru_d"	,m[ 5][10]);	mc("ru_rd"	,m[ 5][11]);
	mc("r_lu"	,m[ 6][9]);	mc("r_u"	,m[ 6][10]);	mc("r_ru"	,m[ 6][11]);
	mc("r_l"	,m[ 7][9]);	mc("r_c"	,m[ 7][10]);	mc("r_r"	,m[ 7][11]);
	mc("r_ld"	,m[ 8][9]);	mc("r_d"	,m[ 8][10]);	mc("r_rd"	,m[ 8][11]);
	mc("rd_lu"	,m[ 9][9]);	mc("rd_u"	,m[ 9][10]);	mc("rd_ru"	,m[ 9][11]);
	mc("rd_l"	,m[10][9]);	mc("rd_c"	,m[10][10]);	mc("rd_r"	,m[10][11]);
	mc("rd_ld"	,m[11][9]);	mc("rd_d"	,m[11][10]);	mc("rd_rd"	,m[11][11]);
	mc("ddr_lu"	,m[12][9]);	mc("ddr_u"	,m[12][10]);	mc("ddr_ru"	,m[12][11]);
	mc("ddr_l"	,m[13][9]);	mc("ddr_c"	,m[13][10]);	mc("ddr_r"	,m[13][11]);
	mc("ddr_ld"	,m[14][9]);	mc("ddr_d"	,m[14][10]);	mc("ddr_rd"	,m[14][11]);

	mc("rru_lu"	,m[ 3][12]);	mc("rru_u"	,m[ 3][13]);	mc("rru_ru"	,m[ 3][14]);
	mc("rru_l"	,m[ 4][12]);	mc("rru_c"	,m[ 4][13]);	mc("rru_r"	,m[ 4][14]);
	mc("rru_ld"	,m[ 5][12]);	mc("rru_d"	,m[ 5][13]);	mc("rru_rd"	,m[ 5][14]);
	mc("rr_lu"	,m[ 6][12]);	mc("rr_u"	,m[ 6][13]);	mc("rr_ru"	,m[ 6][14]);
	mc("rr_l"	,m[ 7][12]);	mc("rr_c"	,m[ 7][13]);	mc("rr_r"	,m[ 7][14]);
	mc("rr_ld"	,m[ 8][12]);	mc("rr_d"	,m[ 8][13]);	mc("rr_rd"	,m[ 8][14]);
	mc("rrd_lu"	,m[ 9][12]);	mc("rrd_u"	,m[ 9][13]);	mc("rrd_ru"	,m[ 9][14]);
	mc("rrd_l"	,m[10][12]);	mc("rrd_c"	,m[10][13]);	mc("rrd_r"	,m[10][14]);
	mc("rrd_ld"	,m[11][12]);	mc("rrd_d"	,m[11][13]);	mc("rrd_rd"	,m[11][14]);

}

int main(int argc, char * argv[])
{
//	printf("%d\n",sizeof(int));
//	test_skbrackets::bar();
	setlocale(LC_ALL,"");

	NN=9;
	alphabetsize=1;
	alphabet[0]="1";
	alphabet[1]="2";
	alphabet[2]="3";
	alphabet[3]="4";
	alphabet[4]="5";
	alphabet[5]="6";
	alphabet[6]="7";
	alphabet[7]="8";
	alphabet[8]="9";


#if 1 // 9x9
	#define NINExNINE 4
	/*
	 0 void
	 1 create9x9with_4quads
	 2 create9x9with_diags - some bug in inputdata
	 3 create9x9 hard
	 4 create9x9figured hard
	 */

	#if NINExNINE==0 //default
		int sumas[9][9]=
		{
		{0,0,0, 0,0,0, 0,0,0},
		{0,0,0, 0,0,0, 0,0,0},
		{0,0,0, 0,0,0, 0,0,0},

		{0,0,0, 0,0,0, 0,0,0},
		{0,0,0, 0,0,0, 0,0,0},
		{0,0,0, 0,0,0, 0,0,0},

		{0,0,0, 0,0,0, 0,0,0},
		{0,0,0, 0,0,0, 0,0,0},
		{0,0,0, 0,0,0, 0,0,0},
		};
	#elif NINExNINE==4
		create9x9figured();
		int sumas[9][9]=
		{
		{0,0,8, 0,9,0, 5,0,0},
		{0,1,2, 0,0,6, 0,0,5},
		{0,7,0, 0,0,0, 0,0,4},

		{8,0,0, 0,0,0, 0,0,0},
		{0,6,0, 0,0,0, 0,1,0},
		{0,0,0, 9,0,0, 0,4,0},

		{0,0,0, 3,0,0, 0,0,0},
		{0,0,0, 0,0,7, 0,0,0},
		{2,0,0, 0,0,0, 0,0,0},
		};
	#elif NINExNINE==1
		create9x9with_4quads();
		int sumas[9][9]=
		{
		{0,0,0, 0,0,7, 0,4,3},
		{0,0,0, 0,0,0, 0,0,9},
		{0,0,6, 0,0,0, 0,8,0},

		{0,1,0, 0,0,4, 0,7,0},
		{0,0,0, 0,0,0, 0,0,0},
		{0,0,0, 5,0,0, 0,0,0},

		{0,3,0, 0,0,0, 0,0,0},
		{2,0,4, 0,0,8, 5,0,0},
		{6,0,0, 0,0,0, 0,0,8},
		};
	#elif NINExNINE==2
		create9x9with_diags();
		int sumas[9][9]=
		{
		{4,8,6, 0,0,0, 0,1,0},
		{1,2,5, 7,0,6, 4,0,9},
		{7,3,9, 0,0,0, 0,2,0},

		{0,5,0, 0,3,0, 0,7,0},
		{0,0,0, 6,0,7, 0,0,0},
		{0,1,0, 0,5,0, 0,4,0},

		{0,6,0, 0,0,0, 0,9,0},
		{2,0,1, 3,0,9, 8,0,4},
		{0,9,0, 0,0,0, 0,6,0},
		};
	#elif NINExNINE==3
		create9x9();
		int sumas[9][9]=
		{
		{8,0,0, 0,0,0, 0,0,0},
		{0,0,3, 6,0,0, 0,0,0},
		{0,7,0, 0,9,0, 2,0,0},

		{0,5,0, 0,0,7, 0,0,0},
		{0,0,0, 0,4,5, 7,0,0},
		{0,0,0, 1,0,0, 0,3,0},

		{0,0,1, 0,0,0, 0,6,8},
		{0,0,8, 5,0,0, 0,1,0},
		{0,9,0, 0,0,0, 4,0,0},
		};
	#endif
		
	set9x9(sumas);
#else //15x15
	#if 0
		create15x15();
		int sumas[15][15]=
		{
		{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
		{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
		{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},

		{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
		{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
		{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},

		{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
		{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
		{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},

		{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
		{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
		{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},

		{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
		{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
		{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
		};
		set15x15(sumas);
	#else
		create15x15();
		int sumas[15][15]=
		{
		{0,0,0, 0,0,0, 5,0,0, 0,9,0, 0,0,0},
		{0,0,0, 0,0,0, 0,0,4, 0,5,2, 0,0,0},
		{0,0,0, 0,2,0, 8,0,6, 0,0,0, 0,0,0},

		{9,0,4, 0,0,0, 0,0,0, 0,0,0, 3,1,0},
		{2,0,0, 0,0,0, 0,0,0, 0,0,0, 5,7,0},
		{0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},

		{7,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,8},
		{1,0,5, 0,0,0, 0,0,0, 0,0,0, 2,0,0},
		{8,9,2, 0,0,0, 0,0,0, 0,0,0, 4,0,5},

		{5,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
		{0,0,0, 0,0,0, 0,0,0, 0,0,0, 6,5,1},
		{6,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0},
		
		{0,0,0, 0,7,4, 0,0,0, 6,0,0, 0,0,0},
		{0,0,0, 0,0,0, 0,1,9, 0,0,0, 0,0,0},
		{0,0,0, 0,3,0, 0,6,7, 0,0,9, 0,0,0},
		};
		set15x15(sumas);
	#endif
#endif


pr_level = (PR_level)(pr_null
//	+pr_eval		//только для функции eval
	+pr_inout		//вывод найденных решений
//	+pr_perebor		//перебор (в evalrec)
//	+pr_perebres	//вывод результатов при переборе
//	+pr_titles		//заголовки функций ядра
//	+pr_titleres	//вывод результатов сразу после заголовка
//	+pr_titles2		//используется в u2
//	+pr_titleres2	//используется в u2
//	+pr_u1			//используется в u1
//	+pr_changes		//вывод изменений
//	+pr_det1		//детализация 1го уровня
//	+pr_det2		//детализация 2го уровня
//	+pr_ss			//вывод структур всех множеств в каждом цикле
//	+pr_mainss		//вывод структур всех множеств в самом начале
);
st_level = (ST_level)(st_null
//	|st_eval		//только для функции eval
//	|st_titles		//после окончания функций ядра
//	|st_perebor		//если не нашлось решений
//	|st_perebor_	//после предположения
//	|st_det1		//детализация 1го уровня
//	|st_changes		//изменения
//	|st_inout		//самый последний getchar()
);

//--------------------------------------
	try{
		eval();//оставляет 1 вариант решения
	}catch(...)
	{
		printf("неизвестное исключение");
		if(st_level)getchar();
		return 2;
	}
	if(pr_level&pr_inout)printf("найдено %d решений\n",rezcount);
	//if(pr_level&pr_mainss)printss(suss);
	//if(pr_level>=pr_inout)print();
	if(st_level&st_inout)getchar();

	return rezcount==0 ? 1 : 0;
}