/*
 * Thread.cpp
 *
 */
#include <cassert>

#include "Input/Thread.hpp"

namespace Input
{

Thread::Thread(ReaderPtrNN reader, Core::Types::AlertsFifo &output):
  reader_(reader),
  output_(&output)
{
}

void Thread::operator()(void)
{
  assert( output_!=NULL );
  assert( reader_.get()!=NULL );
  // TODO
}


} // namespace Input
