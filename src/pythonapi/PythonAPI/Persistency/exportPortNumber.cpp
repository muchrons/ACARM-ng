/*
 * exportPortNumber.cpp
 *
 */
#include "Persistency/PortNumber.hpp"
#include "PythonAPI/Python.hpp"
#include "PythonAPI/Persistency/exportPortNumber.hpp"

using namespace boost::python;
using namespace ::Persistency;

namespace PythonAPI
{
namespace Persistency
{

void exportPortNumber(void)
{
  class_<PortNumber>("PortNumber", init<PortNumber::Numeric>())
    .def("get", &PortNumber::get)
  ;
}

} // namespace Persistency
} // namespace PythonAPI
