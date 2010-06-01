/*
 * CUrlInit.cpp
 *
 */
#include <cassert>

#include "curlpp/cURLpp.hpp"
#include "Logger/Logger.hpp"
#include "System/Threads/SafeInitLocking.hpp"
#include "System/AtExit.hpp"
#include "Filter/IPBlackList/CUrlInit.hpp"

namespace Filter
{
namespace IPBlackList
{

namespace
{
// these will be automatically initialized in compile-time
// so there is no race possible.
SYSTEM_MAKE_STATIC_SAFEINIT_MUTEX(mutex);
long g_count     =0;
bool g_exitIsDone=false;
bool g_terminated=false;

// dealocator for AtExit
struct CUrlUninit: public System::AtExitResourceDeallocator
{
  virtual void deallocate(void)
  {
    Logger::Node log("filter.ipblacklist.curluninit");
    System::Threads::SafeInitLock lock(mutex);
    LOGMSG_INFO(log, "termination called from AtExit");
    assert(g_terminated==false && "termination called before deallocate()");
    g_exitIsDone=true;
    // terminate ONLY if there are no more instances of CUrlInit present
    if(g_count==0)
      terminate();
    else
      assert(g_terminated==false && "oops - curl terminated too soon");
  }

  static void terminate(void)
  {
    Logger::Node log("filter.ipblacklist.curluninit");
    LOGMSG_INFO(log, "terminating curl library");
    assert(g_terminated==false && "oops - curl has been already terminated");
    g_terminated=true;
    curlpp::terminate();
  }
};
} // unnamed namespace


CUrlInit::CUrlInit(void):
  log_("filter.ipblacklist.curlinit")
{
  System::Threads::SafeInitLock lock(mutex);
  if(g_count==0)
  {
    LOGMSG_INFO(log_, "initializing curl library");
    // initialize cURL++
    curlpp::initialize();
    // create deinitializer to AtExit, if it has not been reviously created.
    if(g_exitIsDone==false)
    {
      LOGMSG_INFO(log_, "registering automatic deallocator uppon exit");
      System::AtExit::TDeallocPtr ptr(new CUrlUninit);
      System::AtExit::registerDeallocator(ptr);
    }
    else
    {
      LOGMSG_INFO(log_, "re-registration of curl - skipping AtExit registration");
      g_terminated=false;
    }
  } // if(intialize)

  assert(g_terminated==false && "something's wrong here...");
  ++g_count;
}

CUrlInit::~CUrlInit(void)
{
  System::Threads::SafeInitLock lock(mutex);
  --g_count;
  // if we're the last instance and AtExit() has already finished,
  // we have to do clean-up ourselfs
  if(g_count==0 && g_exitIsDone==true)
  {
    LOGMSG_INFO(log_, "we're the last instance - calling termination");
    CUrlUninit::terminate();
  }
  else
    assert(g_terminated==false && "oops - curl terminated too soon");
}

} // namespace IPBlackList
} // namespace Filter
