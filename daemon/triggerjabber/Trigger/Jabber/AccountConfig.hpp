/*
 * AccountConfig.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_JABBER_ACCOUNTCONFIG_HPP_FILE
#define INCLUDE_TRIGGER_JABBER_ACCOUNTCONFIG_HPP_FILE

#include <string>

namespace Trigger
{
namespace Jabber
{

/** \brief configuration of Jabber's account.
 */
class AccountConfig
{
public:
  /** \brief create configuration.
   *  \param server   Jabber server.
   *  \param login    user's Jabber login.
   *  \param password password used for authorization.
   */
  AccountConfig(const std::string server, const std::string login, const std::string &password):
    server_(server),
    login_(login),
    password_(password)
  {
  }

  /** \brief gets Jabber's server.
   *  \return Jaberr's server.
   */
  const std::string getServer(void) const
  {
    return server_;
  }
  /** \brief gets user's login.
   *  \return user's login.
   */
  const std::string getLogin(void) const
  {
    return login_;
  }
  /** \brief gets user's password.
   *  \return user's password for given User's ID.
   */
  const std::string &getPassword(void) const
  {
    return password_;
  }

private:
  std::string server_;
  std::string login_;
  std::string password_;
}; // class AccountConfig

} // namespace Jabber
} // namespace Trigger

#endif
