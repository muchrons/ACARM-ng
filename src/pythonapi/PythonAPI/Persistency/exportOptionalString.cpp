/*
 * exportOptionalString.cpp
 *
 */
#include "PythonAPI/Python.hpp"
#include "PythonAPI/Persistency/OptionalString.hpp"
#include "PythonAPI/Persistency/exportOptionalString.hpp"

using namespace boost::python;

namespace PythonAPI
{
namespace Persistency
{

void exportOptionalString(void)
{
  scope inOptionalString=
    class_<OptionalString>("OptionalString", init<const char *>())
      .def("get", &OptionalString::get)
    ;
}

} // namespace Persistency
} // namespace PythonAPI
