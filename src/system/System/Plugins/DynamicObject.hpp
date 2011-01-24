/*
 * DynamicObject.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_PLUGINS_DYNAMICOBJECT_HPP_FILE
#define INCLUDE_SYSTEM_PLUGINS_DYNAMICOBJECT_HPP_FILE

/* public header */

#include <dlfcn.h>
#include <cassert>

#include "System/Plugins/Handle.hpp"
#include "System/Plugins/Symbol.hpp"
#include "System/Plugins/ExceptionCannotReadSymbol.hpp"

namespace System
{
namespace Plugins
{

/** \brief helper object owning handle returned by dlopen().
 */
class DynamicObject
{
public:
  /** \brief create proxy-object.
   *  \param h handle to take (cannot be NULL).
   *  \note object takes ownership of 'h'.
   */
  explicit DynamicObject(HandlePtrNN h):
    h_(h)
  {
  }

  /** \brief gets given symbol from handle.
   *  \param name name of the symbol to get.
   *  \return symbol with a given name.
   *
   *  call throws an error if symbol gy a given name does not exist.
   *
   *  \warning since this is C++ interface to C calls types are NOT checked
   *           thus user must ensure that symbol with a given name really has
   *           supplied signature - otherwise undefined behavior may occure.
   */
  template<typename S>
  Symbol<S> getSymbol(const std::string &name)
  {
    dlerror();  // clean previous error message
    S s;
    // NOTE: this is a workarround, since in C99 casting from void* to function
    //       pointer is an undefined behavior.
    *(void**)(&s)=dlsym( get(), name.c_str() );
    const char *err=dlerror();
    if(err!=NULL)
      throw ExceptionCannotReadSymbol(SYSTEM_SAVE_LOCATION, name, err);
    return Symbol<S>(h_, name, s);
  }

private:
  void *get(void)
  {
    assert(h_.get()!=NULL);
    assert(h_->get()!=NULL);
    return h_->get();
  }

  HandlePtrNN h_;
}; // class DynamicObject

} // namespace Plugins
} // namespace System

#endif
