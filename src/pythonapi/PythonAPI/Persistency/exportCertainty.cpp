/*
 * exportCertainty.cpp
 *
 */
#include <boost/python.hpp>

#include "Persistency/Certainty.hpp"
#include "PythonAPI/Persistency/exportCertainty.hpp"

using namespace boost::python;
using ::Persistency::Certainty;

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
