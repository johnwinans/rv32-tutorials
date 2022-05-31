#include <stdlib.h>

extern "C" {
  extern void (*__CTOR_LIST__)() ;
  extern void (*__DTOR_LIST__)() ;
  void callConstructors();
  void callDestructors();
}

void callConstructors()
{
  void (**constructor)() = &__CTOR_LIST__ ;
  int total = *(int *)constructor ;
  constructor++ ;
  while(total){
          (*constructor)() ;
          total-- ;
          constructor++ ;
      }
}
void callDestructors()
{
  void (**deconstructor)() = &__DTOR_LIST__ ;
  int total = *(int *)deconstructor ;
  deconstructor++ ;
  while(total){
          (*deconstructor)() ;
          total-- ;
          deconstructor++ ;
      }
}




void* operator new (unsigned size)
{
	return malloc(size);
}
void * operator new[] (unsigned size)
{
	return malloc(size);
}
void operator delete (void*p)
{
	free(p);
}
void operator delete[] (void*p)
{
	free(p);
}

extern "C" void __cxa_pure_virtual (){}
void * __dso_handle=0;
extern "C" void  __cxa_atexit(){}
