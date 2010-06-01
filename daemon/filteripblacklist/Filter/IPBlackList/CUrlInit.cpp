/*
 * CUrlInit.cpp
 *
 */
#include <cassert>

#include "curlpp/cURLpp.hpp"
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
long count=0;

// dealocator for AtExit
struct CUrlUninit: public System::AtExitResourceDeallocator
{
  virtual void deallocate(void)
  {
    System::Threads::SafeInitLock lock(mutex);
    // TODO: this assertion sometimes fails - add mechanism that after exit() las user
    //       will release CURL's data.
    assert(count==0 && "ooops - someone is still using cURL++...");
    curlpp::terminate();
  }
};
} // unnamed namespace


CUrlInit::CUrlInit(void)
{
  System::Threads::SafeInitLock lock(mutex);
  if(count==0)
  {
    // initialize cURL++
    curlpp::initialize();
    // create deinitializer to AtExit.
    System::AtExit::TDeallocPtr ptr(new CUrlUninit);
    System::AtExit::registerDeallocator(ptr);
  }
  ++count;
}

CUrlInit::~CUrlInit(void)
{
  System::Threads::SafeInitLock lock(mutex);
  --count;
}

} // namespace IPBlackList
} // namespace Filter
