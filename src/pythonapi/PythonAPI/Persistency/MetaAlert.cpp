/*
 * MetaAlert.cpp
 *
 */
#include <cassert>

#include "Commons/Convert.hpp"
#include "Persistency/GraphNode.hpp"
#include "Persistency/Facades/StrAccess/Params.hpp"
#include "Persistency/Facades/StrAccess/DefaultHandleMap.hpp"
#include "Persistency/Facades/StrAccess/MainDispatcher.hpp"
#include "PythonAPI/GlobalLock.hpp"
#include "PythonAPI/Persistency/MetaAlert.hpp"


namespace StrFc=Persistency::Facades::StrAccess;


namespace PythonAPI
{
namespace Persistency
{

MetaAlert::MetaAlert(::Persistency::ConstGraphNodePtrNN gn):
  log_("pythonapi.persistency.metaalert"),
  node_(gn)
{
  LOGMSG_DEBUG_S(log_)<<"string access facade for node "<<node_->getMetaAlert()->getID().get();
}


namespace
{
/** \brief helper callback handle - saves results returned by dispatcher.
 */
struct CallbackHandle
{
  bool collectionSize(size_t size)
  {
    assert(out_.get()==NULL);
    out_=OptionalString(Commons::Convert::to<std::string>(size));
    return true;
  }

  bool value(const std::string &v)
  {
    out_=OptionalString(v);
    return true;
  }

  bool nullOnPath(const std::string &/*where*/)
  {
    assert(out_.get()==NULL);
    return true;
  }

  OptionalString out_;
}; // strict CallbackHandle
} // unnamed namespace


OptionalString MetaAlert::get(const std::string &path) const
{
  GlobalLock lock;
  try
  {
    typedef StrFc::Params<StrFc::DefaultHandleMap, CallbackHandle> Params;
    // compute response
    CallbackHandle cb;
    Params         p(StrFc::Path(path), cb);
    StrFc::MainDispatcher::process(node_, p);
    // return computed value, if set
    if(cb.out_.get()!=NULL)
    {
      LOGMSG_DEBUG_S(log_)<<"element '"<<path<<"' of node "<<node_->getMetaAlert()->getID().get()<<" is "<<cb.out_.get();
      return cb.out_;
    }
    // by default return NULL
    LOGMSG_DEBUG_S(log_)<<"element '"<<path<<"' of node "<<node_->getMetaAlert()->getID().get()<<" returns NULL";
    return OptionalString();
  }
  catch(const StrFc::Exception &ex)
  {
    LOGMSG_ERROR_S(log_)<<"facade error accessing element '"<<path<<"': "<<ex.what();
  }
  catch(const std::exception &ex)
  {
    LOGMSG_ERROR_S(log_)<<"generic error accessing element '"<<path<<"': "<<ex.what();
  }
  // on error return empty string
  return OptionalString();
}

} // namespace Persistency
} // namespace PythonAPI
