/*
 * exportMetaAlert.cpp
 *
 */
#include "PythonAPI/Python.hpp"
#include "PythonAPI/Persistency/MetaAlert.hpp"
#include "PythonAPI/Persistency/exportMetaAlert.hpp"

using namespace boost::python;

namespace PythonAPI
{
namespace Persistency
{

void exportMetaAlert(void)
{
  scope inMetaAlert=
    class_<MetaAlert>("MetaAlert", no_init)
      .def("get", &MetaAlert::get)
    ;
}

} // namespace Persistency
} // namespace PythonAPI
