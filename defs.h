#include "common.h"

/*все определения в defs.cpp*/

/*я сам плохо понимаю (спустя год) как это работает*/
class Cell;
class sNum;
struct Suset//sudoku set
{
	Cell * c[NNmax];//самостоятельно
	Cell * cs[NNmax];//тоже что c, только сортированное
		//используется при создании пересечений в U1init()
	void sort();
	sNum * n[NNmax];/*как контейнер для n*/
	int cc[NNmax], nc[NNmax];//размер-1 конфигурации, в которой учавствует 
		//соответствующая клетка или цифра
		//размер конфигурации - кол-во множеств, к которым принадлежит данная клетка или цифра
		//(для цифр это 1)
	// и их нельзя запихнуть в Cell & sNum т.к. нельзя запихнуть в Cell 
	//т.к. 1 Cell содержится в разных сусетах, а cc - относится персонально к одному сусету
	const char * name;//самостоятельно
	//
	Suset(string alphabet[]);
	Suset();//никогда не используется
	~Suset();
	Suset(const Suset &x);
	void copy(Suset *);
		//проверяет, чтобы совпадали имена sourc'a & dest'a
		//для n - Suset это контейнер
		//c - не копируется
private:
	Suset & operator=(Suset &);
};
class sNum
{
	friend class Cell;
	int x;
public:
	int nbit;//readonly
	Suset * inset;//указатель на сусет, в котором этот sNum
	int insetpos;//номер этого sNum'a в этом сусете
	const char * name;
	//-----
	sNum();
	//конструктор копирования - по умолчанию
	void copy(sNum * pn);
	operator int()const	{	return x;	}
	sNum & operator=(int xx);
	sNum & operator&=(int xx)
	{	return *this = *this&xx;	}
	sNum & operator|=(int xx)
	{	return *this = *this|xx;	}
	sNum & disable(int i);
private:
	sNum & operator=(const sNum &);
};
#define MAX_CELLS_IN_SUSET 100
class Cell
{
	friend class sNum;
	int x;
public:
	int nbit;//readonly
	//макс число сусетов, в котором может состоять целл
	Suset * inset[MAX_CELLS_IN_SUSET];//указатель на сусеты, в которых этот Cell
	int insetpos[MAX_CELLS_IN_SUSET];//номера позиций в этих сусетах
	const char * name;
	//-----
	Cell();//устанавливает все NN битов в 1
	//конструктор копирования - по умолчанию
	void copy(Cell * pc);
		//проверяет, чтобы совпадали имена sourc'a & dest'a
		//копирует x & nbit
	operator int()const	{	return x;	}
	Cell & operator=(int xx);
		//присваивает x и соответственно меняет nbit
		//корректно работает только после конструирования
	Cell & operator&=(int xx)
	{	return *this = *this&xx;	}
	Cell & operator|=(int xx)
	{	return *this = *this|xx;	}
	Cell & disable(int i);
private:
	Cell & operator=(const Cell &);//через opertor=(int) это делается
};
