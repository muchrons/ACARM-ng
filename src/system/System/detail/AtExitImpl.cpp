/*
 * AtExitImpl.cpp
 *
 */
#include <iostream>
#include <cassert>
#include <stdlib.h>

#include "System/detail/AtExitImpl.hpp"
#include "System/FunctionName.hpp"

using namespace std;
using namespace boost;


namespace System
{
namespace detail
{

AtExitImpl::AtExitImpl(void):
  deallocationDone_(false)
{
}

AtExitImpl::~AtExitImpl(void)
{
  assert(deallocationDone_ && "deallocateAll() has not been called. "
                              "this suggest resource leak.");
  assert(deallocators_.size()==0 && "something registered after "
                                    "deallocateAll()");
}

void AtExitImpl::deallocateAll(void)
{
  assert(!deallocationDone_ && "deallocateAll() called more than once");
  deallocationDone_=true;

  // process each and every element on the stack
  while( !deallocators_.empty() )
  {
    try
    {
      assert( deallocators_.size()> 0u );
      // free one more resource :)
      assert( deallocators_.top().get()!=NULL );
      deallocators_.top()->deallocate();    // deallocate
      deallocators_.pop();                  // destroy an object
    }
    catch(const std::exception &ex)
    {
      cerr<<"EXCEPTION in "<<SYSTEM_FUNCTION_NAME<<"(): "<<ex.what()<<endl;
      assert(!"fatal error - exception from AtExitResourceDeallocator::deallocate() has been thrown");
      // following code should not be part of the library...
      //abort();
    }
    catch(...)
    {
      cerr<<"fatal error - exception has been thrown in "<<SYSTEM_FUNCTION_NAME<<endl;;
      assert(!"fatal error - exception from AtExitResourceDeallocator::deallocate() has been thrown");
      // following code should not be part of the library...
      //abort();
    }
  }
  // ensure each dealocation will take place just once
  assert( deallocators_.size()==0u );
}

void AtExitImpl::registerDeallocator(AtExit::TDeallocPtr ptr)
{
  assert(!deallocationDone_ && "deallocateAll() already called");

  Elem elem( ptr.release() );   // convert to container-safe ptr type
  deallocators_.push(elem);     // add to container
}

} // namespace detail
} // namespace System
