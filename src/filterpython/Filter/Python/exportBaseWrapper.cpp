/*
 * exportBaseWrapper.cpp
 *
 */
#include "PythonAPI/Python.hpp"
#include "Filter/Python/BaseWrapper.hpp"
#include "Filter/Python/exportBaseWrapper.hpp"

using namespace boost::python;

namespace Filter
{
namespace Python
{

void exportBaseWrapper(void)
{
  class_<BaseWrapper, boost::noncopyable, boost::shared_ptr<BaseWrapper> >("BaseWrapper", init<>())
    //TODO: dead code
    //.def("filter", &BaseWrapper::filter)
    //.def("filterImpl", &BaseWrapper::filterImpl, (arg("ma")))
  ;
}

} // namespace Python
} // namespace Filter
