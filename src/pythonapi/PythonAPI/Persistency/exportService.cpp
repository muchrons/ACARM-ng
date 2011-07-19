/*
 * exportService.cpp
 *
 */
#include "Persistency/Service.hpp"
#include "PythonAPI/Python.hpp"
#include "PythonAPI/Persistency/exportService.hpp"
#include "PythonAPI/Persistency/exportLimitedString.hpp"

using namespace boost::python;
using namespace ::Persistency;

namespace PythonAPI
{
namespace Persistency
{

void exportService(void)
{
  scope inService=
    class_<Service, boost::noncopyable>("Service", init<Service::Name, PortNumber, Service::Protocol, ConstReferenceURLPtr>())
//      .def("getName", &ReferenceURL::getName, return_value_policy<copy_const_reference>())
//      .def("getURL",  &ReferenceURL::getURL,  return_value_policy<copy_const_reference>())
    ;

//  exportLimitedString<Service::Name::size>("Name");
//  exportLimitedString<Service::Protocol::size>("URL");
}

} // namespace Persistency
} // namespace PythonAPI
