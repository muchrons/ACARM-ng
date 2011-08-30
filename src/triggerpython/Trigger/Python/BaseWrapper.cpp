/*
 * BaseWrapper.cpp
 *
 */
#include "PythonAPI/GlobalLock.hpp"
#include "Trigger/Python/BaseWrapper.hpp"

namespace Trigger
{
namespace Python
{

BaseWrapper::BaseWrapper(void)
{
}

/*
BaseWrapper::BaseWrapper(const boost::reference_wrapper<const Trigger::Python::BaseWrapper>::type &other):
  Base(other)
{
}
*/

void BaseWrapper::triggerImpl(PythonAPI::Persistency::MetaAlert ma)
{
  PythonAPI::GlobalLock lock;

  // get derived virtual call
  boost::python::override f=this->get_override("triggerImpl");
  if(!f)
  {
    LOGMSG_ERROR(log_, "no virtual override for method triggerImpl()");
    throw ExceptionNoImplementation(SYSTEM_SAVE_LOCATION, "triggerImpl()");
  }

  // run implementation
  LOGMSG_DEBUG_S(log_)<<"passing node "<<ma.get("metaalert.id").get()<<" to python implementation";
  f(ma);
  LOGMSG_DEBUG_S(log_)<<"back from python processing of node "<<ma.get("metaalert.id").get();
}

} // namespace Python
} // namespace Trigger
