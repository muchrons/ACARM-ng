/*
 * Loader.cpp
 *
 */
#include "Logger/Logger.hpp"
#include "Commons/Filesystem/isFileSane.hpp"
#include "Plugins/Loader.hpp"
#include "Plugins/Registrator.hpp"
#include "Plugins/ExceptionRegistrationError.hpp"

using namespace System::Plugins;
using namespace Commons::Filesystem;
namespace fs=boost::filesystem;

namespace Plugins
{

Loader::Loader(const boost::filesystem::path &dir):
  log_("plugins.loader")
{

  LOGMSG_INFO_S(log_)<<"loading plugins from directory '"<<dir<<"'";
  const fs::directory_iterator end=fs::directory_iterator();
  // loop thought all elements in the directory
  int count=0;
  for(fs::directory_iterator it(dir); it!=end; ++it)
  {
    LOGMSG_DEBUG_S(log_)<<"checking file: '"<<*it<<"'";
    // sanity check
    if( !isFileSane(*it) )
    {
      LOGMSG_DEBUG(log_, "file does not look sane");
      continue;
    }
    // extension check
    if( fs::extension(*it)==".plugin" )
    {
      LOGMSG_DEBUG(log_, "file does not look like a plugin (required extension is '.plugin')");
      continue;
    }

    // ok - if all the basic conditions are meet, load the plugin
    LOGMSG_DEBUG(log_, "file looks like a plugin - trying to use it as one");
    loadPlugin(*it);
    ++count;
    LOGMSG_INFO_S(log_)<<"plugin '"<<*it<<"' loaeded";
  } // for(files)

  LOGMSG_INFO_S(log_)<<count<<" plugins loaded";
}

void Loader::loadPlugin(const fs::path &plugin)
{
  // open this plugin
  LOGMSG_DEBUG_S(log_)<<"opening plugin '"<<plugin<<"'";
  DynamicObject dyn=builder_.open(plugin);
  // read symbol that registers plugin
  typedef const char*(*Func)(void*);
  const char *name="register_plugin";
  LOGMSG_DEBUG_S(log_)<<"trying to obtain symbol '"<<name<<"'";
  Symbol<Func> init=dyn.getSymbol<Func>(name);
  // sanity check for a symbol
  if( init.get()==NULL )
    throw ExceptionRegistrationError(SYSTEM_SAVE_LOCATION, plugin.string(), "symbol is NULL");

  // ok - now try to register
  LOGMSG_DEBUG_S(log_)<<"registering plugin with provided function";
  const char *error=(*init)(&dyn);
  if(error!=NULL)
  {
    LOGMSG_FATAL_S(log_)<<"unable to register plugin '"<<plugin<<"' - registration failed with message: "<<error;
    throw ExceptionRegistrationError(SYSTEM_SAVE_LOCATION, plugin.string(), error);
  }
  LOGMSG_DEBUG_S(log_)<<"registering call's done (no error)";
}

} // namespace Plugins
