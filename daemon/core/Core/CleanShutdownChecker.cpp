/*
 * CleanShutdownChecker.cpp
 *
 */
#include <cstring>
#include <cassert>

#include "Logger/Logger.hpp"
#include "Persistency/IO/create.hpp"
#include "Persistency/IO/Connection.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Persistency/IO/DynamicConfig.hpp"
#include "Core/CleanShutdownChecker.hpp"

using namespace Persistency::IO;

namespace Core
{

namespace
{
struct ParamIO
{
  ParamIO(void):
    log_("core.cleanshutdownchecker.paramio"),
    conn_( Persistency::IO::create() ),
    t_( conn_->createNewTransaction("clean_shutdown_check_transaction") ),
    dc_( conn_->dynamicConfig("Clean Shutdown Checker", t_) ),
    key_("last status")
  {
    assert( dc_.get()!=NULL );
  }

  void commit(void)
  {
    LOGMSG_DEBUG(log_, "commiting changes");
    t_.commit();
  }

  DynamicConfig::ValueNULL read(void)
  {
    LOGMSG_DEBUG_S(log_)<<"reading value for key '"<<key_<<"'";
    assert( dc_.get()!=NULL );
    return dc_->read(key_);
  }

  void write(const DynamicConfig::Value &v)
  {
    LOGMSG_DEBUG_S(log_)<<"writing value '"<<v.get()<<"' for key '"<<key_<<"'";
    assert( dc_.get()!=NULL );
    dc_->write(key_, v);
  }

private:
  Logger::Node          log_;
  ConnectionPtrNN       conn_;
  Transaction           t_;
  DynamicConfigAutoPtr  dc_;
  const char           *key_;
}; // struct ParamIO
} // unnamed namespace


CleanShutdownChecker::CleanShutdownChecker(void):
  log_("core.cleanshutdownchecker"),
  confirmed_(false)
{
  LOGMSG_DEBUG(log_, "checking previous value");
  ParamIO pio;
  // check last saved state
  const DynamicConfig::ValueNULL prev=pio.read();
  if( prev.get()==NULL )
  {
    LOGMSG_DEBUG(log_, "previous value is NULL (i.e. first run)");
    lastRunSuccessful_=true;
  }
  else
  {
    assert( prev.get()->get()!=NULL );
    if( strcmp(prev.get()->get(), "confirmed exit")==0 )
    {
      LOGMSG_DEBUG(log_, "last run finished without errors");
      lastRunSuccessful_=true;
    }
    else
    {
      LOGMSG_ERROR_S(log_)<<"last run did NOT finished successfully - last satatus was: '"<<prev.get()->get()<<
                            "' - if this is not due to power/hardware failure you should report this problem";
      lastRunSuccessful_=false;
    }
  }
  // write new state
  pio.write("started");
  pio.commit();
}

CleanShutdownChecker::~CleanShutdownChecker(void)
{
  try
  {
    ParamIO pio;
    if(confirmed_)
    {
      LOGMSG_DEBUG(log_, "normal, confirmed exit");
      pio.write("confirmed exit");
    }
    else
    {
      LOGMSG_DEBUG(log_, "application exit on error");
      pio.write("exit without confirmation");
    }
    pio.commit();
  }
  catch(const std::exception &ex)
  {
    // nothing more can be done here
    LOGMSG_ERROR_S(log_)<<"exception throw in d-tor: '"<<ex.what()<<"' (ignored)";
  }
}

void CleanShutdownChecker::confirmCleanShutdown(void)
{
  LOGMSG_DEBUG(log_, "confirming exit");
  ParamIO pio;
  pio.write("confirmed, not closed yet");
  confirmed_=true;
  pio.commit();
  LOGMSG_DEBUG(log_, "exit confirmed");
}

bool CleanShutdownChecker::wasLastRunSuccessful(void) const
{
  return lastRunSuccessful_;
}

} // namespace Core
