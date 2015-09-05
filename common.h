#include <string>
using std::string;

#pragma warning (disable:4996)
#define USENS using namespace
#define FOREACH(T,i,cont) for(T i=cont.begin(); i!=cont.end(); i++)

//максимальное число клеток в множестве
//#if (sizeof(int)==4)//препроцессор - не компилятор, и он ничего не знает о платформе
//разве что какие-нибудь константы...
enum{NNmax=30};
//#else
//enum{NNmax=16}
//#endif
extern int NN;//цифры 0..NN-1
/*defs.cpps*/
int CountBit(int);
/*defs.cpps*/

