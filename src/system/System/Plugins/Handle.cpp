/*
 * Handle.cpp
 *
 */
#include <dlfcn.h>

#include "System/Plugins/Handle.hpp"

namespace System
{
namespace Plugins
{

Handle::Handle(void *h):
  h_(h)
{
  if(h_==NULL)
    throw ExceptionPointerIsNULL(SYSTEM_SAVE_LOCATION, "h");
}

Handle::~Handle(void)
{
  dlclose( get() );
}

} // namespace Plugins
} // namespace System
