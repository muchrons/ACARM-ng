/*
 * exportGenericLimitedString.hpp
 *
 */
#ifndef INCLUDE_PYTHONAPI_PERSISTENCY_EXPORTGENERICLIMITEDSTRING_HPP_FILE
#define INCLUDE_PYTHONAPI_PERSISTENCY_EXPORTGENERICLIMITEDSTRING_HPP_FILE

#include <string>

#include "PythonAPI/Python.hpp"

namespace PythonAPI
{
namespace Persistency
{
namespace detail
{

/** \brief exports generic string class.
 */
template<typename TLimStr>
void exportGenericLimitedString(const char *name)
{
  using namespace boost::python;
  class_<TLimStr>(name, init<>())
    .def(init<const char*>())
    .def(init<const std::string>())
    .def("get", &TLimStr::get)
  ;
} // exportGenericLimitedString()

} // namespace detail
} // namespace Persistency
} // namespace PythonAPI

#endif
