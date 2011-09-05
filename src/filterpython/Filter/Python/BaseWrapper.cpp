/*
 * BaseWrapper.cpp
 *
 */
#include "PythonAPI/GlobalLock.hpp"
#include "PythonAPI/ExceptionHandle.hpp"
#include "Filter/Python/BaseWrapper.hpp"

namespace Filter
{
namespace Python
{

  /*
void BaseWrapper::filterImpl(PythonAPI::Persistency::MetaAlert ma)
{
  PythonAPI::GlobalLock lock;
  try
  {
    // get derived virtual call
    boost::python::override f=this->get_override("filterImpl");
    if(!f)
    {
      LOGMSG_ERROR(log_, "no virtual override for method filterImpl()");
      throw ExceptionNoImplementation(SYSTEM_SAVE_LOCATION, "filterImpl()");
    }

    // run implementation
    LOGMSG_DEBUG_S(log_)<<"passing node "<<ma.get("metaalert.id").get()<<" to python implementation";
    f(ma);
    LOGMSG_DEBUG_S(log_)<<"back from python processing of node "<<ma.get("metaalert.id").get();
  }
  catch(const boost::python::error_already_set&)
  {
    PythonAPI::ExceptionHandle eh;
    eh.rethrow();
  }
}
*/

} // namespace Python
} // namespace Filter
