/*
 * PersistencyConfig.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_PERSISTENCYCONFIG_HPP_FILE
#define INCLUDE_CONFIGIO_PERSISTENCYCONFIG_HPP_FILE

/* public header */

#include <string>
#include <stdint.h>

#include "ConfigIO/Exception.hpp"


namespace ConfigIO
{

/** \brief class representing configuration of persistency storage.
 */
class PersistencyConfig
{
public:
  /** \brief type used to represent port number.
   */
  typedef uint16_t PortNumber;

  /** \brief creates persistency configuration.
   *  \param type persistency type.
   *  \param user user name to lgoin with.
   *  \param pass password to use for user.
   *  \param host host name/address to connect to.
   *  \param port port number to use when connecting.
   */
  PersistencyConfig(const std::string &type,
                    const std::string &user,
                    const std::string &pass,
                    const std::string &host,
                    PortNumber         port);

  /** \brief gets persistency storage type.
   *  \return persitency type name.
   */
  const std::string getType(void) const
  {
    return type_;
  }

  /** \brief gets user name (login).
   *  \return user name to login with.
   */
  const std::string getUser(void) const
  {
    return user_;
  }

  /** \brief gets password to autenticate with.
   *  \return password to persistency storage.
   */
  const std::string getPassword(void) const
  {
    return pass_;
  }

  /** \brief gets host name to connect to.
   *  \return host name to connect to.
   */
  const std::string getHost(void) const
  {
    return host_;
  }

  /** \brief gets port number to connect to on given host.
   *  \return port to connect to.
   */
  PortNumber getPortNumber(void) const
  {
    return port_;
  }

private:
  std::string type_;
  std::string user_;
  std::string pass_;
  std::string host_;
  PortNumber  port_;
}; // class PersistencyConfig


/** \brief exception informing about incorrect port number.
 */
struct ExceptionInvalidPortNumber: public Exception
{
  /** \brief create exception of invalid port number.
   *  \param where place where problem has been detected.
   *  \param port  port that has been decided to be invalid.
   */
  ExceptionInvalidPortNumber(const char                    *where,
                             PersistencyConfig::PortNumber  port);
}; // struct ExceptionInvalidPortNumber

} // namespace ConfigIO

#endif
