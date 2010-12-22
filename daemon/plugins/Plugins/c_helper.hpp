/*
 * c_helper.hpp
 *
 */
#ifndef INCLUDE_PLUGINS_C_HELPER_HPP_FILE
#define INCLUDE_PLUGINS_C_HELPER_HPP_FILE

/* public header */

#include <cstdlib>
#include <cstring>
#include <cassert>

#include "Logger/Logger.hpp"
#include "Plugins/Registrator.hpp"

namespace Plugins
{

/** \brief registration interface - C wrapper to make user-code simpler.
 */
template<typename TSingleton, typename TBuilder>
char *c_helper(void *dynObjVoidPtr)
{
  // we must ensure no exception crosses border of this function
  try
  {
    const Logger::Node log("plugins.chelper");
    try
    {
      typedef System::Plugins::DynamicObject DynObj;
      DynObj *dyn=static_cast<DynObj*>(dynObjVoidPtr);
      assert(dyn!=NULL);
      assert( typeid(System::Plugins::DynamicObject).name()==typeid(*dyn).name() );
      Registrator<TSingleton, TBuilder> reg(*dyn);
      LOGMSG_DEBUG(log, "object registered");
      return NULL;
    }
    catch(const std::exception &ex)
    {
      LOGMSG_ERROR_S(log)<<"exception during registration process: "<<ex.what();
      return strdup( ex.what() );
    }
  }
  catch(...)
  {
    return strdup("unknown exception");
  }

  assert(!"code never reaches here");
} // c_helper()

} // namespace Plugins

#endif
