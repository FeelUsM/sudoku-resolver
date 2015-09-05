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
void create9x9()
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
	newSuset("lu");	newSuset("u");	newSuset("ru");
	newSuset("l");	newSuset("c");	newSuset("r");
	newSuset("ld");	newSuset("d");	newSuset("rd");
		
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
void create9x9with_diags()
{
	create9x9();
	newSuset("d1");	newSuset("d2");
	link9("d1","lu_lu","lu_c","lu_rd","c_lu","c_c","c_rd","rd_lu","rd_c","rd_rd");
	link9("d2","ld_ld","ld_c","ld_ru","c_ld","c_c","c_ru","ru_ld","ru_c","ru_ru");
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
	
	//create9x9with_diags();
	create9x9();
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
	set9x9(sumas);

pr_level = (PR_level)(pr_null
//	+pr_eval		//только для функции eval
	+pr_inout		//вывод найденных решений
	+pr_perebor		//перебор (в evalrec)
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