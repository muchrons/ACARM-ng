/*
 * exportLimitedNULLString.cpp
 *
 */
#ifndef INCLUDE_PYTHONAPI_PERSISTENCY_EXPORTLIMITEDNULLSTRING_HPP_FILE
#define INCLUDE_PYTHONAPI_PERSISTENCY_EXPORTLIMITEDNULLSTRING_HPP_FILE

#include "Commons/LimitedNULLString.hpp"
#include "PythonAPI/Persistency/detail/exportGenericLimitedString.hpp"


namespace PythonAPI
{
namespace Persistency
{

/** \brief exports LimitedNULLString class.
 *  \param name name of the module to be exported.
 */
template<uint16_t N>
void exportLimitedNULLString(const char *name)
{
  using namespace boost::python;
  using namespace Commons;
  detail::exportGenericLimitedString< Commons::LimitedNULLString<N> >(name);
} // exportLimitedNULLString()

} // namespace Persistency
} // namespace PythonAPI

#endif
