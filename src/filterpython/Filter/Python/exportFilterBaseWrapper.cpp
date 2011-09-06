/*
 * exportFilterBaseWrapper.cpp
 *
 */
#include "PythonAPI/Python.hpp"
#include "Filter/Python/FilterBaseWrapper.hpp"
#include "Filter/Python/exportFilterBaseWrapper.hpp"

using namespace boost::python;

namespace Filter
{
namespace Python
{

void exportFilterBaseWrapper(void)
{
  class_<FilterBaseWrapper, boost::noncopyable, boost::shared_ptr<FilterBaseWrapper> >("FilterBaseWrapper", init<>())
    //.def("filter", &FilterBaseWrapper::filter)
    //.def("filterImpl", &FilterBaseWrapper::filterImpl, (arg("ma")))
  ;
}

} // namespace Python
} // namespace Filter
