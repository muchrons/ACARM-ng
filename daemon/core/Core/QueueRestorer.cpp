/*
 * QueueRestorer.cpp
 *
 */
#include <cassert>

#include "Logger/Logger.hpp"
#include "Persistency/IO/Connection.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Core/QueueRestorer.hpp"

using namespace Persistency::IO;


namespace Core
{

QueueRestorer::QueueRestorer(Core::Types::UniqueNodesFifo &queue)
{
  const Logger::Node log("core.queuerestorer");
  try
  {
    LOGMSG_INFO(log, "restoring data base's content");

    // preapre environment
    BackendFactory::FactoryPtr conn=create();
    assert( conn.get()!=NULL );
    Transaction                t( conn->createNewTransaction("meta_alerts_queue_restore") );
    RestorerAutoPtr            rest=conn->restorer(t);
    // read data base content
    Restorer::NodesVector      tmp;
    LOGMSG_DEBUG(log, "reading meta-alerts");
    rest->restoreAllInUse(tmp);
    t.commit();
    // add all elements to queue
    LOGMSG_DEBUG_S(log)<<tmp.size()<<" meta-alerts read from persistency; appending to queue";
    for(Restorer::NodesVector::const_iterator it=tmp.begin(); it!=tmp.end(); ++it)
      queue.push(*it);

    LOGMSG_INFO_S(log)<<"restoring "<<tmp.size()<<" elements successful";
  }
  catch(const std::exception &ex)
  {
    LOGMSG_ERROR_S(log)<<"unable to restore data base's content; error was: "
                       <<ex.what()<<"; continuing any way...";
  }
}

} // namespace Core
