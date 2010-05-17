/*
 * AccountConfig.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_ACCOUNTCONFIG_HPP_FILE
#define INCLUDE_TRIGGER_GG_ACCOUNTCONFIG_HPP_FILE

#include <string>

#include "Trigger/GG/UserID.hpp"

namespace Trigger
{
namespace GG
{

/** \brief configuration of Gadu-Gadu's account.
 */
class AccountConfig
{
public:
  /** \brief create configuration.
   *  \param uid      user's GG-UIN.
   *  \param password password used for authorization.
   */
  AccountConfig(UserID uid, const std::string &password):
    uid_(uid),
    password_(password)
  {
  }

  /** \brief gets user's ID.
   *  \return user's ID.
   */
  UserID getUserID(void) const
  {
    return uid_;
  }
  /** \brief gets user's password.
   *  \return user's password for given User's ID.
   */
  const std::string &getPassword(void) const
  {
    return password_;
  }

private:
  UserID      uid_;
  std::string password_;
}; // class AccountConfig

} // namespace GG
} // namespace Trigger

#endif
