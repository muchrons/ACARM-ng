/*
 * Main.cpp
 *
 */
#include <cassert>

#include "Logger/Logger.hpp"
#include "Commons/SharedPtrNotNULL.hpp"
#include "Persistency/IO/create.hpp"
#include "Core/Processors.hpp"
#include "Core/Sources.hpp"
#include "Core/Main.hpp"

namespace Core
{

namespace
{
// helper f-ction for returning ponter to connection
inline Persistency::IO::ConnectionPtrNN makeConnection(void)
{
  Persistency::IO::BackendFactory::FactoryPtr tmp=Persistency::IO::create();
  return tmp;
} // makeConnection()
} // unnamed namespace


Main::Main(void):
  log_("core.main"),
  nullSignals_(NULL),                       // ignore all interruptions at the begining
  restorer_( makeConnection(), queue_ ),    // restore data content
  threads_(queue_),                         // run processing threads
  // (non-explicit initialization is being done here)
  signals_(&threads_)                       // after all initialization is done, register real handles
{
  LOGMSG_INFO(log_, "core is up and running");
}

Main::~Main(void)
{
  LOGMSG_INFO(log_, "destroying core");
}

void Main::waitUntilDone(void)
{
  threads_.waitUntilDone();                 // wait while threads are working
  csc_.confirmCleanShutdown();              // mark this as clean exit (i.e. no exception, no
                                            // segv, no power cut, etc...)
}

void Main::stop(void)
{
  threads_.stop();
}

} // namespace Core
