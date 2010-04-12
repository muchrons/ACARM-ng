/*
 * timestampFromString.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_TIMESTAMPFROMSTRING_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_TIMESTAMPFROMSTRING_HPP_FILE

#include <string>

#include "Persistency/Timestamp.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

/** \brief converts given string to timestamp value.
 *  \param str string represeting date and time. string is assumed to be in UTC.
 *  \return timestamp values repreesnted by given string.
 */
Timestamp timestampFromString(const std::string &str);

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
