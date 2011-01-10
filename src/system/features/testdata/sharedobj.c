//
// gcc -g3 -Wall -shared -o sharedobj.so -fPIC sharedobj.c
//

#include <stdlib.h>

int g_int=42;

long testFunctionSymbol(const char *c)
{
  if(c==NULL)
    return 0;
  if(*c=='\0')
    return 0;
  return 1+testFunctionSymbol(c+1);
}
