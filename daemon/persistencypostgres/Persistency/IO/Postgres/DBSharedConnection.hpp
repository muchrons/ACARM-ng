/*
 * DBSharedConnection.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_DBSHAREDCONNECTION_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_DBSHAREDCONNECTION_HPP_FILE

#include <boost/shared_ptr.hpp>

namespace Persistency
{
namespace IO
{
namespace Postgres
{

/** \brief class reprenseting shared connection to data base.
 *
 *  class has boost::shared_ptr<>-like copy semantics. it does not
 *  guarantee safe usage from multiple threads.
 */
class DBSharedConnection
{
public:
  // TODO
private:
  // TODO
};

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
