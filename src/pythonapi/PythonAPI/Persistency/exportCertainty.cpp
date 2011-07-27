/*
 * exportCertainty.cpp
 *
 */
#include "Persistency/Certainty.hpp"
#include "PythonAPI/Python.hpp"
#include "PythonAPI/Persistency/exportCertainty.hpp"

using namespace boost::python;
using namespace ::Persistency;

namespace PythonAPI
{
namespace Persistency
{

void exportCertainty(void)
{
  class_<Certainty>("Certainty", init<double>())
    .def("get", &Certainty::get)
  ;
}

} // namespace Persistency
} // namespace PythonAPI
