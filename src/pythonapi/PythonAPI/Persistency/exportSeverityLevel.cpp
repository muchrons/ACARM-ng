/*
 * exportSeverityLevel.cpp
 *
 */
#include "Persistency/SeverityLevel.hpp"
#include "PythonAPI/Python.hpp"
#include "PythonAPI/Persistency/exportSeverityLevel.hpp"

using namespace boost::python;
using namespace ::Persistency;

namespace PythonAPI
{
namespace Persistency
{

void exportSeverityLevel(void)
{
  scope inSeverityLevel=
  class_<SeverityLevel>("SeverityLevel", init<SeverityLevel::EType>())
    .def("toInt", &SeverityLevel::toInt)
  ;

  // accessible from python as <module>.SeverityLevel.EType.<level>
  enum_<SeverityLevel::EType>("EType")
    .value("DEBUG",  SeverityLevel::DEBUG)
    .value("INFO",   SeverityLevel::INFO)
    .value("LOW",    SeverityLevel::LOW)
    .value("MEDIUM", SeverityLevel::MEDIUM)
    .value("HIGH",   SeverityLevel::HIGH)
  ;
}

} // namespace Persistency
} // namespace PythonAPI
