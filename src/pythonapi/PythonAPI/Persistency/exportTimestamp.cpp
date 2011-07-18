/*
 * exportTimestamp.cpp
 *
 */
#include "Persistency/Timestamp.hpp"
#include "PythonAPI/Python.hpp"
#include "PythonAPI/Persistency/exportTimestamp.hpp"

using namespace boost::python;
using namespace ::Persistency;

namespace PythonAPI
{
namespace Persistency
{

void exportTimestamp(void)
{
  class_<Timestamp>("Timestamp", init<time_t>())
//    .def("get", &Certainty::get)
  ;
}

} // namespace Persistency
} // namespace PythonAPI
