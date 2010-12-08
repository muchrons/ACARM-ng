/*
 * Strategy.cpp
 *
 */
#include <cassert>

#include "Base/Threads/Lock.hpp"
#include "Trigger/Compose/Summary.hpp"
#include "Trigger/Jabber/Strategy.hpp"
#include "Trigger/Jabber/MessageIO.hpp"

using namespace std;
using namespace Base::Threads;

namespace Trigger
{
namespace Jabber
{
namespace
{
struct PingThread
{
  explicit PingThread(detail::StrategyIO &s):
    log_("trigger.jabber.pingthread"),
    s_(&s)
  {
  }

  void operator()(void)
  {
    LOGMSG_DEBUG(log_, "ping thread started");

    bool quit=false;
    // ping server every 60[s]
    while(!quit)
    {
      try
      {
        // wait a while before next polling (NOTE: this try{}catch MUST start with
        // interruptable event, in case pinging or discarding messages would throw).
        boost::this_thread::sleep( boost::posix_time::seconds(60) );
        assert(s_!=NULL);
        s_->ping();                             // ping server
        s_->discardIncommingMessages();         // throw away anything that might have arrived
      }
      catch(const boost::thread_interrupted &)
      {
        // ok - thread has been interrupted
        LOGMSG_DEBUG(log_, "interruption requested - exiting");
        quit=true;
      }
      catch(const std::exception &ex)
      {
        // hmmm...
        LOGMSG_WARN_S(log_)<<"exception caught: "<<ex.what()
                           <<"; if this is connection-related problem system will automatically reconnect soon";
      }
    } // while(true)

    LOGMSG_DEBUG(log_, "leaving scope gentelly");
  }

private:
  Logger::Node        log_;
  detail::StrategyIO *s_;
}; // struct PingThread
} // unnamed namespace

Strategy::Strategy(const std::string &name, const Config &cfg):
  Trigger::Simple::Strategy("jabber", name, cfg.getThresholdConfig() ),
  io_(cfg),
  pingThread_( PingThread(io_) )
{
}

void Strategy::triggerImpl(const ConstNode &n)
{
  // prepare data
  stringstream  ss;
  Compose::Summary::append(ss, n);

  io_.send( ss.str() );
}

} // namespace Jabber
} // namespace Trigger
