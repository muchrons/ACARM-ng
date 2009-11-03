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

// TODO: test
// TODO: comment

namespace ConfigIO
{

class PersistencyConfig
{
public:
  typedef uint16_t PortNumber;

  PersistencyConfig(const std::string &user,
                    const std::string &pass,
                    const std::string &host,
                    PortNumber         port);

  const std::string getUser(void) const
  {
    return user_;
  }

  const std::string getPassword(void) const
  {
    return pass_;
  }

  const std::string getHost(void) const
  {
    return host_;
  }

  PortNumber getPortNumber(void) const
  {
    return port_;
  }

private:
  std::string user_;
  std::string pass_;
  std::string host_;
  PortNumber  port_;
}; // class PersistencyConfig

struct ExceptionInvalidPortNumber: public Exception
{
  ExceptionInvalidPortNumber(const char                    *where,
                             PersistencyConfig::PortNumber  port);
}; // struct ExceptionInvalidPortNumber

} // namespace ConfigIO

#endif
