#include <map>
using std::map;

#include "defs.h"

/*---kernel.cpp---*/
enum PR_level{
	pr_null		=0x0000,
	pr_eval		=0x0001,//только для функции eval
	pr_inout	=0x0002,//вывод найденных решений
	pr_perebor	=0x0004,//перебор (в evalrec)
	pr_perebres	=0x0008,//вывод результатов при переборе
	pr_titles	=0x0010,//заголовки функций ядра
	pr_titleres	=0x0020,//вывод результатов сразу после заголовка
	pr_titles2	=0x0040,//используется в u2
	pr_titleres2=0x0080,//используется в u2
	pr_u1		=0x0100,//используется в u1
	pr_changes	=0x0200,//вывод изменений
	pr_det1		=0x0400,//детализация 1го уровня
	pr_det2		=0x0800,//детализация 2го уровня
	pr_ss		=0x1000,//вывод структур всех множеств в каждом цикле
	pr_mainss	=0x2000,//вывод структур всех множеств в самом начале
};
extern PR_level pr_level;
enum ST_level{
	st_null		=0,
	st_eval		=1,//только для функции eval
	st_titles	=2,//после окончания функций ядра
	st_perebor	=4,//если не нашлось решений
	st_perebor_	=16,//после предположения
	st_det1		=8,//детализация 1го уровня
	st_changes	=32,//изменения
	st_inout	=64,//самый последний getchar()
};
extern ST_level st_level;

typedef map<string,Cell> MapC;
typedef map<string,Suset> MapSs;

void newCell(const char * name);
void newSuset(const char * name);
void link(const char * susname, int n/*номер позиции клетки в сусете*/, const char * cellname);
void setc(const char * name, int x/*позиция в алфавите*/);
void eval();
extern int rezcount;

/*---main.cpp---*/
extern string alphabet[NNmax];
//имена цифр
extern int alphabetsize;
//предполагается, что длины имен всех цифр имеют этот размер

void print(const Cell &);//отладочный вывод; и требует getchar()
void print(/*const*/ MapC &);//выводит все, что есть на данный момент
//const закомментирован из-за особенностей map'а
//отвратительно, что не определен константный оператор [] у мэпа
void printss(Suset * *);
void printss(Suset * s1, Suset *s2=0, Suset *s3=0);
//void printss(const set<Suset *>& sss);
void printss(MapSs & sss);
void printconf(int x);//выводит точки и галочки в зависимости от битов
int printdot(int n);//выводит ------------
