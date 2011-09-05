/*
 * BaseWrapper.cpp
 *
 */
#include "Logger/Logger.hpp"
#include "PythonAPI/GlobalLock.hpp"
#include "PythonAPI/ExceptionHandle.hpp"
#include "Filter/Python/BaseWrapper.hpp"

// TODO: think about some meanof reducing c&p code (limitation is number of arguments to f()).

namespace Filter
{
namespace Python
{

bool BaseWrapper::isEntryInterestingImpl(PyMetaAlert thisEntry, DataPtr data) const
{
  PythonAPI::GlobalLock lock;
  try
  {
    // get derived virtual call
    boost::python::override f=this->get_override("isEntryInterestingImpl");
    if(!f)
    {
      LOGMSG_ERROR(log_, "no virtual override for method isEntryInterestingImpl()");
      throw ExceptionNoImplementation(SYSTEM_SAVE_LOCATION, "isEntryInterestingImpl()");
    }

    // run implementation
    LOGMSG_DEBUG_S(log_)<<"passing node "<<thisEntry.get("metaalert.id").get()<<" to python implementation";
    const bool ret=f(thisEntry, data);
    LOGMSG_DEBUG_S(log_)<<"back from python processing of node "<<thisEntry.get("metaalert.id").get();
    return ret;
  }
  catch(const boost::python::error_already_set&)
  {
    PythonAPI::ExceptionHandle eh;
    eh.rethrow();
  }
  assert(!"code never reaches here");
  throw std::logic_error("code should never reach here");
}


std::string BaseWrapper::getMetaAlertNameImpl(PyMetaAlert thisEntry,
                                              DataPtr     thisEntryData,
                                              PyMetaAlert otherEntry,
                                              DataPtr     otherEntryData) const
{
  PythonAPI::GlobalLock lock;
  try
  {
    // get derived virtual call
    boost::python::override f=this->get_override("getMetaAlertNameImpl");
    if(!f)
    {
      LOGMSG_ERROR(log_, "no virtual override for method getMetaAlertNameImpl()");
      throw ExceptionNoImplementation(SYSTEM_SAVE_LOCATION, "getMetaAlertNameImpl()");
    }

    // run implementation
    LOGMSG_DEBUG_S(log_)<<"passing node "<<thisEntry.get("metaalert.id").get()<<" to python implementation";
    LOGMSG_DEBUG_S(log_)<<"passing node "<<otherEntry.get("metaalert.id").get()<<" to python implementation";
    const std::string ret=f(thisEntry, thisEntryData, otherEntry, otherEntryData);
    LOGMSG_DEBUG_S(log_)<<"back from python processing of node "<<thisEntry.get("metaalert.id").get();
    return ret;
  }
  catch(const boost::python::error_already_set&)
  {
    PythonAPI::ExceptionHandle eh;
    eh.rethrow();
  }
  assert(!"code never reaches here");
  throw std::logic_error("code should never reach here");
}


bool BaseWrapper::canCorrelateImpl(PyMetaAlert thisEntry,
                                   DataPtr     thisEntryData,
                                   PyMetaAlert otherEntry,
                                   DataPtr     otherEntryData) const
{
  PythonAPI::GlobalLock lock;
  try
  {
    // get derived virtual call
    boost::python::override f=this->get_override("canCorrelateImpl");
    if(!f)
    {
      LOGMSG_ERROR(log_, "no virtual override for method canCorrelateImpl()");
      throw ExceptionNoImplementation(SYSTEM_SAVE_LOCATION, "canCorrelateImpl()");
    }

    // run implementation
    LOGMSG_DEBUG_S(log_)<<"passing node "<<thisEntry.get("metaalert.id").get()<<" to python implementation";
    LOGMSG_DEBUG_S(log_)<<"passing node "<<otherEntry.get("metaalert.id").get()<<" to python implementation";
    const bool ret=f(thisEntry, thisEntryData, otherEntry, otherEntryData);
    LOGMSG_DEBUG_S(log_)<<"back from python processing of node "<<thisEntry.get("metaalert.id").get();
    return ret;
  }
  catch(const boost::python::error_already_set&)
  {
    PythonAPI::ExceptionHandle eh;
    eh.rethrow();
  }
  assert(!"code never reaches here");
  throw std::logic_error("code should never reach here");
}


DataPtr BaseWrapper::makeThisEntryUserDataImpl(PyMetaAlert n) const
{
  PythonAPI::GlobalLock lock;
  try
  {
    // get derived virtual call
    boost::python::override f=this->get_override("makeThisEntryUserDataImpl");
    if(!f)
    {
      LOGMSG_ERROR(log_, "no virtual override for method makeThisEntryUserDataImpl()");
      throw ExceptionNoImplementation(SYSTEM_SAVE_LOCATION, "makeThisEntryUserDataImpl()");
    }

    // run implementation
    LOGMSG_DEBUG_S(log_)<<"passing node "<<n.get("metaalert.id").get()<<" to python implementation";
    const DataPtr ret=f(n);
    LOGMSG_DEBUG_S(log_)<<"back from python processing of node "<<n.get("metaalert.id").get();
    return ret;
  }
  catch(const boost::python::error_already_set&)
  {
    PythonAPI::ExceptionHandle eh;
    eh.rethrow();
  }
  assert(!"code never reaches here");
  throw std::logic_error("code should never reach here");
}

} // namespace Python
} // namespace Filter
