/*
 * Loader.cpp
 *
 */
#include <cassert>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Commons/Filesystem/isFileSane.hpp"
#include "Commons/Filesystem/isDirectorySane.hpp"
#include "Plugins/Loader.hpp"

using namespace System::Plugins;
using namespace Commons::Filesystem;

namespace fs=boost::filesystem;

namespace Plugins
{

Loader::Loader(const boost::filesystem::path &dir):
  log_("plugins.loader"),
  count_(0)
{
  try
  {
    loadAll(dir);
  }
  catch(const System::Plugins::ExceptionCannotOpenSharedObject &ex)
  {
    LOGMSG_FATAL_S(log_)<<"cannot open shared object: "<<ex.what();
    throw ExceptionInvalidPlugin(SYSTEM_SAVE_LOCATION, ex.what() );
  }
  catch(const System::Plugins::ExceptionCannotReadSymbol &ex)
  {
    LOGMSG_FATAL_S(log_)<<"cannot read symbol: "<<ex.what();
    throw ExceptionInvalidPlugin(SYSTEM_SAVE_LOCATION, ex.what() );
  }
  catch(const System::Exception &ex)
  {
    LOGMSG_FATAL_S(log_)<<"generic system error: "<<ex.what();
    throw ExceptionInvalidPlugin(SYSTEM_SAVE_LOCATION, ex.what() );
  }
}

void Loader::loadAll(const boost::filesystem::path &dir)
{
  LOGMSG_INFO_S(log_)<<"loading plugins from directory '"<<dir<<"'";
  // sanity check of directory
  if( !isDirectorySane(dir) )
  {
    LOGMSG_FATAL_S(log_)<<"given directory is not sane: "<<dir;
    throw ExceptionInvalidDirectory(SYSTEM_SAVE_LOCATION, dir);
  }

  // prepare deinit set that will be registred to be deallocated in AtExit
  // **BEFORE** anything from plugins will be registered, to prevent problems
  // with global data destruction, whose code has been already deallocated.
  typedef Commons::SharedPtrNotNULL<DeinitSet> DeinitSetPtrNN;
  DeinitSetPtrNN deinitSet(new DeinitSet);

  // loop thought all elements in the directory
  const fs::directory_iterator end=fs::directory_iterator();
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
    if( fs::extension(*it)!=".acmp" )
    {
      LOGMSG_DEBUG(log_, "file does not look like a plugin (required extension is '.acmp')");
      continue;
    }

    // ok - if all the basic conditions are meet, load the plugin
    LOGMSG_DEBUG(log_, "file looks like a plugin - trying to use it as one");
    loadPlugin(*it, *deinitSet);
    ++count_;
    LOGMSG_INFO_S(log_)<<"plugin '"<<*it<<"' loaded";
  } // for(files)

  LOGMSG_INFO_S(log_)<<count_<<" plugins loaded";
}

void Loader::loadPlugin(const boost::filesystem::path &plugin, DeinitSet &deinitSet)
{
  // open this plugin
  LOGMSG_DEBUG_S(log_)<<"opening plugin '"<<plugin<<"'";
  DynamicObject dyn=builder_.open(plugin);
  deinitSet.push_back(dyn);
  LOGMSG_DEBUG_S(log_)<<"unregistering call has been added to deallocation list";
}

} // namespace Plugins
