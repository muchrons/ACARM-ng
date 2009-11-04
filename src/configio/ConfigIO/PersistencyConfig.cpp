/*
 * PersistencyConfig.cpp
 *
 */
#include <sstream>
#include <string>

#include "ConfigIO/PersistencyConfig.hpp"

using namespace std;


namespace ConfigIO
{

PersistencyConfig::PersistencyConfig(const std::string &type,
                                     const std::string &user,
                                     const std::string &pass,
                                     const std::string &host,
                                     PortNumber         port):
  type_(type),
  user_(user),
  pass_(pass),
  host_(host),
  port_(port)
{
  if(port_==0)
    throw ExceptionInvalidPortNumber("PersistencyConfig::PersistencyConfig()",
                                     port_);
}


namespace
{
string makeExceptionString(const char                    *where,
                           PersistencyConfig::PortNumber  port)
{
  stringstream ss;
  ss<<where<<": invalid port number: "<<port;
  return ss.str();
} // makeExceptionString()
} // unnamed namespace

ExceptionInvalidPortNumber::ExceptionInvalidPortNumber(
                                 const char                    *where,
                                 PersistencyConfig::PortNumber  port):
  Exception( makeExceptionString(ensureValidString(where), port) )
{
}

} // namespace ConfigIO
