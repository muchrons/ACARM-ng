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
  // TODO: this does not work since MD5Sum uses named c-tor.
  //       further reading needs to be done for this.
  /*
  class_<MD5Sum>("MD5Sum", init<const char*>())
    .def("get", &MD5Sum::get)
  ;
  */
}

} // namespace Persistency
} // namespace PythonAPI
