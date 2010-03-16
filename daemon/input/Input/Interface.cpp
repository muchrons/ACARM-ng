/*
 * Interface.cpp
 *
 */
#include "Input/Interface.hpp"
#include "Input/Thread.hpp"

namespace Input
{

Interface::Interface(ReaderPtrNN reader, Core::Types::AlertsFifo &output):
  thread_( Thread(reader, output) )
{
  // TODO
}

} // namespace Input
