/*
 * AtExit.cpp
 *
 */
#include <iostream>
#include <stdlib.h>     // atexit()
#include <pthread.h>
#include <boost/checked_delete.hpp>
#include <cassert>

#include "System/AtExit.hpp"
#include "System/AtExitImpl.hpp"
#include "System/Threads/SafeInitLocking.hpp"

using namespace std;
using System::Threads::SafeInitLock;


namespace
{

//
// pointer to implementation class of AtExit. it is static, raw
// pointer since it will be freed by AtExit() uppon application
// termination.
//
// by default this pointer will be NULL when application starts.
// destination object will be assigned uppon first AtExit class
// usage (it registeres this pointer).
//
// compile-time initialized mutex ensures whole construction is
// thread safe from the very begining (before any static constructors
// are run), to the very end.
//
System::AtExitImpl *atExitImpl=NULL;
SYSTEM_MAKE_STATIC_SAFEINIT_MUTEX(g_mutex);

} // unnamed namespace


// helper that will deallocate objects inside queue and AtExitImpl
// class itself.
extern "C"
{
static void cStyleCallForAtExit(void)
{
  // swap this queue with new (NULL - will be allocated, if needed) so
  // that it is possible to register new handlers while processing
  // other handlers.
  System::AtExitImpl *tmp=NULL;
  {
    SafeInitLock lock(g_mutex);
    tmp       =atExitImpl;
    atExitImpl=NULL;
  }
  // pointer can be null in case when this function has been already
  // registered, but AtExitImpl's constructor thrown exception or when
  // adding something to queue from queue itself (ex. Phoenix Singleton).
  if(tmp!=NULL)
  {
    // run dealocation of registered elements
    tmp->deallocateAll();
    // free resource
    boost::checked_delete(tmp);
  }
} // cStyleCallForAtExit()
} // extern "C"


namespace System
{

void AtExit::registerDeallocator(TDeallocPtr p)
{
  SafeInitLock lock(g_mutex);
  if(atExitImpl==NULL)          // not initialized?
    AtExit::init();             // will throw on failure
  assert(atExitImpl!=NULL);

  AtExit::registerInternal(p);  // forward call to implementation
}

void AtExit::init(void)
{
  // lock for this call is made in registerDeallocator()
  assert(atExitImpl==NULL);

  // this call, if there is a problem, can be registered/called
  // multiple times...
  if( atexit(cStyleCallForAtExit)!=0 )
    throw Exception("AtExit::AtExit(): "
                    "unable to register handler in atexit() syscall");

  // sanity check
  assert(atExitImpl==NULL);
  // init global pointer
  atExitImpl=new AtExitImpl;
  // sanity check
  assert(atExitImpl!=NULL);
}

void AtExit::registerInternal(TDeallocPtr p)
{
  // lock for this call is made in registerDeallocator()
  if( p.get()==NULL )
    throw Exception("AtExit::registerInternal(): "
                    "NULL pointer recieved for registration");

  assert(atExitImpl!=NULL);
  atExitImpl->registerDeallocator(p);
}

} // namespace System
