//
// g++ -g3 -Wall -shared -o sharedobj.acmp -fPIC sharedobj.cpp
//

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

namespace
{

const char *registerPlugin(void)
{
  const int fd=creat("_plugin_has_been_initialized_", 0644);
  if(fd==-1)
  {
    perror("internal error - file '_plugin_has_been_initialized_' not created");
    abort();
  }
  close(fd);
  return NULL;
}

const char *g_init=registerPlugin();

} // unnamed namespace
