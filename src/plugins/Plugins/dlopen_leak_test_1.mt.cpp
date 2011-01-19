/*
 * dlopen_leak_test.mt.cpp
 *
 */
#include <iostream>
#include <dlfcn.h>

using namespace std;

int main(void)
{
  cout<<"use valgrind to check memory leaks"<<endl;
  for(int i=0; i<20; ++i)
  {
    void *h=dlopen("/lib/libc.so.6", RTLD_LAZY|RTLD_LOCAL);
    if(h==NULL)
    {
      cerr<<"dlopen() failed"<<endl;
      return 1;
    }
    dlclose(h);
  }
  return 0;
}
