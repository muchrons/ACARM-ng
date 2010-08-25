/*
 * Connection.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_JABBER_CONNECTION_HPP_FILE
#define INCLUDE_TRIGGER_JABBER_CONNECTION_HPP_FILE

#include <boost/noncopyable.hpp>
#include <loudmouth/loudmouth.h>
#include <cassert>

#include "Logger/Node.hpp"
#include "Trigger/Jabber/AccountConfig.hpp"
#include "Trigger/Jabber/AutoSession.hpp"
#include "Trigger/Jabber/ExceptionConnectionError.hpp"
//#include "Trigger/GG/ExceptionSendingError.hpp"

namespace Trigger
{
namespace Jabber
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
  const LmConnection *get(void) const
  {
    assert( sess_.get()!=NULL );
    return sess_.get();
  }
  /** \brief get session structure - non-const version.
   *  \return session pointer.
   *  \note pointer is guaranteed to be non-NULL.
   */
  LmConnection *get(void)
  {
    assert( sess_.get()!=NULL );
    return sess_.get();
  }

private:
  LmConnection *connect(void) const;

  Logger::Node          log_;
  const AccountConfig   cfg_;
  AutoSession           sess_;
}; // class Connection

} // namespace Jabber
} // namespace Trigger

#endif
