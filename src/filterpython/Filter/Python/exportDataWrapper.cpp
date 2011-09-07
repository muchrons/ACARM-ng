/*
 * exportDataWrapper.cpp
 *
 */
#include "PythonAPI/Python.hpp"
#include "Filter/Python/DataWrapper.hpp"
#include "Filter/Python/exportDataWrapper.hpp"

using namespace boost::python;

namespace Filter
{
namespace Python
{

void exportDataWrapper(void)
{
  class_<DataWrapper, boost::noncopyable, boost::shared_ptr<DataWrapper> >("DataWrapper", init<>())
  ;
}

} // namespace Python
} // namespace Filter
