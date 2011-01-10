/*
 * Loader.cpp
 *
 */
#include <memory>
#include <boost/scoped_ptr.hpp>

#include "System/AtExit.hpp"
#include "Logger/Logger.hpp"
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

  const fs::directory_iterator end=fs::directory_iterator();
  // loop thought all elements in the directory
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
    loadPlugin(*it);
    ++count_;
    LOGMSG_INFO_S(log_)<<"plugin '"<<*it<<"' loaded";
  } // for(files)

  LOGMSG_INFO_S(log_)<<count_<<" plugins loaded";
}


namespace
{

// helper class delaying deallocation of given dynamic object until next deallocation round
struct DelayedHandleDeallocation: public System::AtExitResourceDeallocator
{
  void setDynamicObject(std::auto_ptr<DynamicObject> dyn)
  {
    dyn_.reset( dyn.release() );
    assert( dyn.get() ==NULL );
    assert( dyn_.get()!=NULL );
  }

  virtual void deallocate(void)
  {
    dyn_.reset();   // deallocaiton itself
  }

private:
  boost::scoped_ptr<DynamicObject> dyn_;
}; // struct DelayedHandleDeallocation

} // unnamed namespace


void Loader::loadPlugin(const boost::filesystem::path &plugin)
{
  // prepare (delayed) deallocation)
  LOGMSG_DEBUG(log_, "registering delayed deallocator for handle");
  DelayedHandleDeallocation   *ptr=new DelayedHandleDeallocation;
  System::AtExit::TDeallocPtr  deallocator(ptr);
  System::AtExit::registerDeallocator(deallocator);
  LOGMSG_DEBUG(log_, "delayed deallocator registered");

  // open this plugin (it will be registered automatically)
  LOGMSG_DEBUG_S(log_)<<"opening plugin '"<<plugin<<"'";
  std::auto_ptr<DynamicObject> dyn( new DynamicObject( builder_.open(plugin) ) );

  // now attach dynamic object to the deallocator
  LOGMSG_DEBUG(log_, "attaching delayed object deallocator");
  assert(ptr!=NULL);
  ptr->setDynamicObject(dyn);
}

} // namespace Plugins
