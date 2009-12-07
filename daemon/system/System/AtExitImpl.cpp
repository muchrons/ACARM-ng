/*
 * AtExitImpl.cpp
 *
 */
#include <iostream>
#include <cassert>
#include <stdlib.h>

#include "System/AtExitImpl.hpp"

using namespace std;
using namespace boost;


namespace System
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

  // loop throught elements
  for(TList::reverse_iterator it=deallocators_.rbegin();
      it!=deallocators_.rend();
      ++it)
  {
    try
    {
      // free one more resource :)
      (*it)->deallocate();
    }
    catch(...)
    {
      assert(!"fatal error - exception from "
              "AtExitResourceDeallocator::deallocate() has been thrown");
      /*
      // following code should not be part of the library...
      cerr<<"fatal error - exception from "
            "AtExitResourceDeallocator::deallocate() has been thrown\n";
      abort();
      */
    }
  }
  // ensure each dealocation will take place just once
  deallocators_.clear();
}

void AtExitImpl::registerDeallocator(AtExit::TDeallocPtr ptr)
{
  assert(!deallocationDone_ && "deallocateAll() already called");

  TElem elem( ptr.release() );      // convert to container-safe ptr type
  deallocators_.push_back(elem);    // add to container
}

} // namespace System
