/*
 * exportReferenceURL.cpp
 *
 */
#include "Persistency/ReferenceURL.hpp"
#include "PythonAPI/Python.hpp"
#include "PythonAPI/Persistency/exportReferenceURL.hpp"
#include "PythonAPI/Persistency/exportLimitedString.hpp"

using namespace boost::python;
using namespace ::Persistency;

namespace PythonAPI
{
namespace Persistency
{

void exportReferenceURL(void)
{
  scope inReferenceURL=
    class_<ReferenceURL, boost::noncopyable>("ReferenceURL", init<ReferenceURL::Name, ReferenceURL::URL>())
      .def("getName", &ReferenceURL::getName, return_value_policy<copy_const_reference>())
      .def("getURL",  &ReferenceURL::getURL,  return_value_policy<copy_const_reference>())
    ;

  exportLimitedString<128>("Name");
  exportLimitedString<256>("URL");
}

} // namespace Persistency
} // namespace PythonAPI
