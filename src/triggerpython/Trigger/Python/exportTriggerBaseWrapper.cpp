/*
 * exportTriggerBaseWrapper.cpp
 *
 */
#include "PythonAPI/Python.hpp"
#include "Trigger/Python/TriggerBaseWrapper.hpp"
#include "Trigger/Python/exportTriggerBaseWrapper.hpp"

using namespace boost::python;

namespace Trigger
{
namespace Python
{

void exportTriggerBaseWrapper(void)
{
  class_<TriggerBaseWrapper, boost::noncopyable, boost::shared_ptr<TriggerBaseWrapper> >("TriggerBaseWrapper", init<>())
    //.def("trigger", &TriggerBaseWrapper::trigger)
    //.def("triggerImpl", &TriggerBaseWrapper::triggerImpl, (arg("ma")))
  ;
}

} // namespace Python
} // namespace Trigger
