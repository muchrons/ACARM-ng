/*
 * LoginParameters.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_LOGINPARAMETERS_HPP_FILE
#define INCLUDE_TRIGGER_GG_LOGINPARAMETERS_HPP_FILE

#include <boost/scoped_array.hpp>
#include <libgadu.h>

#include "System/EditableCString.hpp"
#include "Trigger/GG/AccountConfig.hpp"

namespace Trigger
{
namespace GG
{

/** \brief class making login struct required by libgadu, basing on given config.
 */
class LoginParameters
{
public:
  /** \brief create libgadu configuration to connect to given account.
   *  \param cfg account's configuation to be used for connectiong.
   */
  explicit LoginParameters(const AccountConfig &cfg);

  /** \brief gets parameters for connection - const version.
   *  \return reference to data used for connecting.
   */
  const gg_login_params &get(void) const
  {
    return p_;
  }
  /** \brief gets parameters for connection - non-const version.
   *  \return reference to data used for connecting.
   */
  gg_login_params &get(void)
  {
    return p_;
  }

private:
  System::EditableCString pass_;
  gg_login_params         p_;
}; // class LoginParameters

} // namespace GG
} // namespace Trigger

#endif
