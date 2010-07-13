/*
 * Connection.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_CONNECTION_HPP_FILE
#define INCLUDE_TRIGGER_GG_CONNECTION_HPP_FILE

#include <boost/noncopyable.hpp>
#include <libgadu.h>
#include <cassert>

#include "Logger/Node.hpp"
#include "Trigger/GG/LoginParameters.hpp"
#include "Trigger/GG/AccountConfig.hpp"
#include "Trigger/GG/AutoSession.hpp"
#include "Trigger/GG/ExceptionConnectionError.hpp"
#include "Trigger/GG/ExceptionSendingError.hpp"

namespace Trigger
{
namespace GG
{

/** \brief class representing (estabilished) connection to GG server.
 */
class Connection: private boost::noncopyable
{
public:
  /** \brief connect to GG-server with given acount.
   *  \param cfg account's configuration to use when connecting.
   */
  explicit Connection(const AccountConfig &cfg);
  /** \brief disconnect from server.
   */
  ~Connection(void);

  /** \brief get session structure - const version.
   *  \return session pointer.
   *  \note pointer is guaranteed to be non-NULL.
   */
  const gg_session *get(void) const
  {
    assert( sess_.get()!=NULL );
    return sess_.get();
  }
  /** \brief get session structure - non-const version.
   *  \return session pointer.
   *  \note pointer is guaranteed to be non-NULL.
   */
  gg_session *get(void)
  {
    assert( sess_.get()!=NULL );
    return sess_.get();
  }

private:
  gg_session *connect(void) const;
  void sendContactsList(void);

  Logger::Node          log_;
  const LoginParameters params_;
  AutoSession           sess_;
}; // class Connection

} // namespace GG
} // namespace Trigger

#endif
