/*
 * exportGenericLimitedString.hpp
 *
 */
#ifndef INCLUDE_PYTHONAPI_PERSISTENCY_EXPORTGENERICLIMITEDSTRING_HPP_FILE
#define INCLUDE_PYTHONAPI_PERSISTENCY_EXPORTGENERICLIMITEDSTRING_HPP_FILE

#include <string>
#include <cassert>

#include "PythonAPI/Python.hpp"

namespace PythonAPI
{
namespace Persistency
{
namespace detail
{

/** \brief exports generic string class.
 *  \param name name of the class, as it should be visible from python script;
 *         must be a non-NULL, compile-time string.
 */
template<typename TLimStr>
void exportGenericLimitedString(const char *name)
{
  assert(name!=NULL && "name must be compile-time, non-NULL string");
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
