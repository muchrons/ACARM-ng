/*
 * Connection.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_CONNECTION_HPP_FILE
#define INCLUDE_TRIGGER_GG_CONNECTION_HPP_FILE

#include <boost/noncopyable.hpp>
#include <libgadu.h>
#include <cassert>

#include "Trigger/GG/LoginParameters.hpp"
#include "Trigger/GG/AccountConfig.hpp"
#include "Trigger/GG/AutoSession.hpp"
#include "Trigger/GG/ExceptionConnectionError.hpp"

// TODO: tests
// TODO: comments

namespace Trigger
{
namespace GG
{

class Connection: private boost::noncopyable
{
public:
  Connection(const AccountConfig &cfg);

  const gg_session *get(void) const
  {
    assert( sess_.get()!=NULL );
    return sess_.get();
  }

  gg_session *get(void)
  {
    assert( sess_.get()!=NULL );
    return sess_.get();
  }

private:
  const LoginParameters params_;
  AutoSession           sess_;
}; // class Connection

} // namespace GG
} // namespace Trigger

#endif
