/*
 * DataBaseID.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_DATABASEIO_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_DATABASEIO_HPP_FILE

#include <map>
#include <boost/noncopyable.hpp>

#include "Base/Threads/Mutex.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

/** \brief type that is used for representation of data base IDs.]
 */
typedef long DataBaseID;

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
