/*
 * LoginParameters.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_LOGINPARAMETERS_HPP_FILE
#define INCLUDE_TRIGGER_GG_LOGINPARAMETERS_HPP_FILE

#include <boost/scoped_array.hpp>
#include <libgadu.h>

#include "Trigger/GG/AccountConfig.hpp"
#include "Trigger/GG/EditableCString.hpp"

// TODO: comments

namespace Trigger
{
namespace GG
{

class LoginParameters
{
public:
  explicit LoginParameters(const AccountConfig &cfg);

  const gg_login_params &get(void) const
  {
    return p_;
  }

  gg_login_params &get(void)
  {
    return p_;
  }

private:
  EditableCString pass_;
  gg_login_params p_;
}; // class LoginParameters

} // namespace GG
} // namespace Trigger

#endif
