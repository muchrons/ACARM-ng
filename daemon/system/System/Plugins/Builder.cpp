/*
 * Builder.cpp
 *
 */
#include <dlfcn.h>

#include "System/Plugins/Builder.hpp"

namespace System
{
namespace Plugins
{

Builder::Builder(const bool makeSymbolsVisible, const bool lazyResolving):
  flags_(0)
{
  flags_|=makeSymbolsVisible?RTLD_GLOBAL:RTLD_LOCAL;
  flags_|=lazyResolving?RTLD_LAZY:RTLD_NOW;
}

DynamicObject Builder::open(const boost::filesystem::path &so) const
{
  dlerror();                                        // remove last error
  void *h=dlopen( so.string().c_str(), flags_ );    // open shared object
  const char *err=dlerror();                        // read error message
  if(err!=NULL)                                     // check for error
    throw ExceptionCannotOpenSharedObject(SYSTEM_SAVE_LOCATION, so, err);
  HandlePtrNN handle( new Handle(h) );
  return DynamicObject(handle);                     // return obtained handle
}

} // namespace Plugins
} // namespace System
