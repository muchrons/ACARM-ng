/*
 * exportLimitedString.cpp
 *
 */
#ifndef INCLUDE_PYTHONAPI_PERSISTENCY_EXPORTLIMITEDSTRING_HPP_FILE
#define INCLUDE_PYTHONAPI_PERSISTENCY_EXPORTLIMITEDSTRING_HPP_FILE

#include "Commons/LimitedString.hpp"
#include "PythonAPI/Persistency/detail/exportGenericLimitedString.hpp"


namespace PythonAPI
{
namespace Persistency
{

/** \brief exports LimitedString class.
 *  \param name name of the module to be exported.
 */
template<uint16_t N>
void exportLimitedString(const char *name)
{
  using namespace boost::python;
  using namespace Commons;
  detail::exportGenericLimitedString< Commons::LimitedString<N> >(name);
} // exportLimitedString()

} // namespace Persistency
} // namespace PythonAPI

#endif
