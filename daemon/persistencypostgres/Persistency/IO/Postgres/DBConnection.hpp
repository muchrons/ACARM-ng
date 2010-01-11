/*
 * DBConnection.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_DBCONNECTION_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_DBCONNECTION_HPP_FILE

#include <string>

namespace Persistency
{
namespace IO
{
namespace Postgres
{

/** \brief class reprenseting connection to data base.
 */
class DBConnection
{
public:
  /** \brief paratmers required for connection to data base.
   */
  struct Parameters
  {
    std::string host_;      ///< name of the host data base server is on.
    std::string dbname_;    ///< name of the data base.
    std::string username_;  ///< user name ot authenticate with.
    std::string password_;  ///< password for given user.
  }; // struct Parameters

  /** \brief create new connection to data base.
   *  \param params parametrs required for openning connection.
   */
  explicit DBConnection(const Parameters &params);

  // TODO: member methods to access data.
private:
  // TODO: postgres connection has to be placed here.
}; // class DBConnection

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
