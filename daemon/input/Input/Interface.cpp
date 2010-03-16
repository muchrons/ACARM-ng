/*
 * Interface.cpp
 *
 */
#include "Logger/Logger.hpp"
#include "Input/Interface.hpp"
#include "Input/Thread.hpp"

namespace Input
{

Interface::Interface(ReaderPtrNN reader, Core::Types::AlertsFifo &output):
  log_( Logger::NodeName("input.interface.", reader->getName().c_str() ) ),
  thread_( Thread(reader, output) )
{
  LOGMSG_INFO(log_, "reader's thread started");
}

Interface::~Interface(void)
{
  try
  {
    stop();
    LOGMSG_INFO(log_, "joining thread");
    thread_.join();
    LOGMSG_INFO(log_, "thread stopped");
  }
  catch(...)
  {
    LOGMSG_FATAL(log_, "exception cought while stopping thread - "
                       "exiting any way...");
  }
}

void Interface::stop(void)
{
  LOGMSG_INFO(log_, "interrupting thread");
  thread_.interrupt();
}

} // namespace Input
