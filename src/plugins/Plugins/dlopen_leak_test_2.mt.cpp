/*
 * dlopen_leak_test_2.mt.cpp
 *
 * example's code base taken from 'man dlopen'.
 */
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main(void)
{
  for(int i=0; i<2; ++i)
  {
    void *handle;
    double (*cosine)(double);
    char *error;

    handle = dlopen("libm.so", RTLD_LAZY);
    if(!handle)
    {
      fprintf(stderr, "%s\n", dlerror());
      return 1;
    }

    dlerror();    /* Clear any existing error */

    /* Writing: cosine = (double (*)(double)) dlsym(handle, "cos");
       would seem more natural, but the C99 standard leaves
       casting from "void *" to a function pointer undefined.
       The assignment used below is the POSIX.1-2003 (Technical
       Corrigendum 1) workaround; see the Rationale for the
       POSIX specification of dlsym(). */

    *(void **) (&cosine) = dlsym(handle, "cos");

    if( (error=dlerror())!=NULL)
    {
      fprintf(stderr, "%s\n", error);
      return 2;
    }

    (*cosine)(2.0);

    dlclose(handle);
  }

  return 0;
}
