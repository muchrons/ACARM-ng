/*
 * LogCallback.cpp
 *
 */
#include <cassert>

#include "Base/Threads/Lock.hpp"
#include "Input/Prelude/LogCallback.hpp"
#include "PreludePP/prelude-log.hpp"

using namespace Base::Threads;

// unnamed namespace for implementaiton details
namespace
{
const Logger::Node *g_node =NULL;
Mutex              *g_mutex=NULL;
} // unnamed namespace

extern "C"
{

static void wrp_Input_Prelude_ignore(int /*level*/, const char* /*log*/)
{
  assert( g_node ==NULL && "unpredicted condition found");
  assert( g_mutex==NULL && "unpredicted condition found");
  // nothing's done here.
} // wrp_Input_Prelude_ignore()

static void wrp_Input_Prelude_loggerCallback(int level, const char *log)
{
  if(log==NULL)     // trust no one!
    log="<NULL>";

  assert( g_node !=NULL && "looks like callback has not ben unregistered");
  assert( g_mutex!=NULL && "looks like callback has not ben unregistered");

  /*
   * this definition is taken from Prelude++:
   *
   * typedef enum {
   *    PRELUDE_LOG_CRIT  = -1,
   *    PRELUDE_LOG_ERR   =  0,
   *    PRELUDE_LOG_WARN  =  1,
   *    PRELUDE_LOG_INFO  =  2,
   *    PRELUDE_LOG_DEBUG  = 3
   * } prelude_log_t;
   */
  assert( ::Prelude::PreludeLog::DEBUG   == 3 );
  assert( ::Prelude::PreludeLog::INFO    == 2 );
  assert( ::Prelude::PreludeLog::WARNING == 1 );
  assert( ::Prelude::PreludeLog::ERROR   == 0 );
  assert( ::Prelude::PreludeLog::CRITICAL==-1 );

  Lock lock(*g_mutex);
  switch(level)
  {
    case  3: LOGMSG_DEBUG(*g_node, log); break;
    case  2: LOGMSG_INFO (*g_node, log); break;
    case  1: LOGMSG_WARN (*g_node, log); break;
    case  0: LOGMSG_ERROR(*g_node, log); break;
    case -1: LOGMSG_FATAL(*g_node, log); break;
    default: LOGMSG_ERROR_S(*g_node)<<"unknown log-level "<<level<<"; message is: "<<log; break;
  } // switch(level)
} // wrp_Input_Prelude_loggerCallback()

} // extern "C"

namespace Input
{
namespace Prelude
{

LogCallback::LogCallback(const char *node):
  log_(node)
{
  LOGMSG_INFO(log_, "registering callback to use common logger in system");

  assert( g_node ==NULL && "LogCallback - more instances have been created!");
  assert( g_mutex==NULL && "LogCallback - more instances have been created!");

  g_node =&log_;
  g_mutex=&mutex_;
  // register callback to prelude
  ::Prelude::PreludeLog::SetCallback( wrp_Input_Prelude_loggerCallback );
  // set other options
  //::Prelude::PreludeLog::SetFlags(0);   // disable loffing to syslog and quiet mode
}

LogCallback::~LogCallback(void)
{
  LOGMSG_WARN(log_, "disconnecting from Prelude++ logger - all logs from "
                    "Prelude++ will be ignored from now on");

  Lock lock(mutex_);
  g_node =NULL;
  g_mutex=NULL;

  // after d-tor is called we may no loger provide loggin environment, so
  // we switch logging off to prevent illegal accesses.
  ::Prelude::PreludeLog::SetCallback( wrp_Input_Prelude_ignore );
}

} // namespace Prelude
} // namespace Input
