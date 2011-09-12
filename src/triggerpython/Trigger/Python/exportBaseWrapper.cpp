/*
 * exportBaseWrapper.cpp
 *
 */
#include "PythonAPI/Python.hpp"
#include "Trigger/Python/BaseWrapper.hpp"
#include "Trigger/Python/exportBaseWrapper.hpp"

using namespace boost::python;

namespace Trigger
{
namespace Python
{

void exportBaseWrapper(void)
{
  class_<BaseWrapper, boost::noncopyable, boost::shared_ptr<BaseWrapper> >("BaseWrapper", init<>())
  ;
}

} // namespace Python
} // namespace Trigger
