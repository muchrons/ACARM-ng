/*
 * Interface.cpp
 *
 */
#include "Logger/Logger.hpp"
#include "Input/Interface.hpp"
#include "Input/Thread.hpp"

namespace Input
{

Interface::Interface(ReaderPtrNN                       reader,
                     Persistency::IO::ConnectionPtrNN  conn,
                     Core::Types::AlertsFifo          &output,
                     CommonDataPtrNN                   commonData):
  log_( Logger::NodeName( "input.interface",
                          Logger::NodeName::removeInvalidChars( reader->getType() ).c_str() ) ),
  thread_( Thread(reader, conn, output, commonData) )
{
  LOGMSG_INFO(log_, "reader's thread started");
}

Interface::~Interface(void)
{
  try
  {
    stop();
    LOGMSG_INFO(log_, "joining thread");
    thread_->join();
    LOGMSG_INFO(log_, "thread stopped");
  }
  catch(...)
  {
    LOGMSG_FATAL(log_, "exception caught while stopping thread - "
                       "exiting any way...");
  }
}

void Interface::stop(void)
{
  LOGMSG_INFO(log_, "interrupting thread");
  thread_->interrupt();
}

} // namespace Input
