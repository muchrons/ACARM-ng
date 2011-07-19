/*
 * exportMD5Sum.cpp
 *
 */
#include "Persistency/MD5Sum.hpp"
#include "PythonAPI/Python.hpp"
#include "PythonAPI/Persistency/exportMD5Sum.hpp"

using namespace boost::python;
using namespace ::Persistency;

namespace PythonAPI
{
namespace Persistency
{

void exportMD5Sum(void)
{
  class_<MD5Sum>("MD5Sum", no_init)
    .def("createFromString", &MD5Sum::createFromString)
    .staticmethod("createFromString")
    .def("get", &MD5Sum::get)
  ;
}

} // namespace Persistency
} // namespace PythonAPI
