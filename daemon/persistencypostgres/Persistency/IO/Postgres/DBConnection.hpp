/*
 * DBConnection.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_DBCONNECTION_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_DBCONNECTION_HPP_FILE

#include <string>
#include <boost/noncopyable.hpp>
#include <pqxx/pqxx>

#include "Logger/Node.hpp"

// TODO: test

namespace Persistency
{
namespace IO
{
namespace Postgres
{

/** \brief class reprenseting connection to data base.
 */
class DBConnection: private boost::noncopyable
{
public:
  /** \brief paratmers required for connection to data base.
   */
  struct Parameters
  {
    /** \brief fast creating of conneciton paramters.
     *  \param host     name of the host data base runs on.
     *  \param dbname   name of the data base to connect to.
     *  \param username name of the user to authorize with.
     *  \param password password for user.
     */
    Parameters(const std::string &host,
               const std::string &dbname,
               const std::string &username,
               const std::string &password):
      host_(host),
      dbname_(dbname),
      username_(username),
      password_(password)
    {
    }

    std::string host_;      ///< name of the host data base server runs on.
    std::string dbname_;    ///< name of the data base.
    std::string username_;  ///< user name ot authenticate with.
    std::string password_;  ///< password for given user.
  }; // struct Parameters

  /** \brief create new connection to data base.
   *  \param params parametrs required for openning connection.
   */
  explicit DBConnection(const Parameters &params);
  /** \brief disconnect from data base.
   */
  ~DBConnection(void);

  /** \brief gets reference to data base connection object.
   *  \return data base connection native driver.
   */
  pqxx::connection &get(void)
  {
    return conn_;
  }

private:
  std::string makeConnectionString(const Parameters &params);

  Logger::Node      log_;
  pqxx::connection  conn_;
  const std::string dbname_;
}; // class DBConnection

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
