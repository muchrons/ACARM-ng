/*
 * exportSeverity.cpp
 *
 */
#include "Persistency/Severity.hpp"
#include "PythonAPI/Python.hpp"
#include "PythonAPI/Persistency/exportSeverity.hpp"

using namespace boost::python;
using namespace ::Persistency;

namespace PythonAPI
{
namespace Persistency
{

void exportSeverity(void)
{
  class_<Severity>("Severity", init<SeverityLevel>())
    .def("getLevel", &Severity::getLevel)
    .def("getName", &Severity::getName)
  ;
}

} // namespace Persistency
} // namespace PythonAPI
