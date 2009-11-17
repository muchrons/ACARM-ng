/*
 * Service.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_SERVICE_HPP_FILE
#define INCLUDE_PERSISTENCY_SERVICE_HPP_FILE

/* public header */

#include <stdint.h>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "Persistency/ReferenceURL.hpp"
#include "Persistency/detail/LimitedString.hpp"
#include "Persistency/detail/LimitedNULLString.hpp"

namespace Persistency
{

/** \brief service's data representation.
 */
class Service: private boost::noncopyable
{
public:
  /** \brief name of the service.
   */
  typedef detail::LimitedString<32>     Name;
  /** \brief port number.
   */
  typedef uint16_t                      Port;
  /** \brief protocol used by sevice.
   */
  typedef detail::LimitedNULLString<32> Protocol;
  /** \brief ensure proper destruction when inherited.
   */
  virtual ~Service(void);
  /** \brief gets service name.
   *  \return name of the service.
   */
  const Name &getName(void) const;
  /** \brief get port number.
   *  \return port number service is running on.
   */
  Port getPort(void) const;
  /** \brief protocol used for communication.
   *  \return prorocol name.
   */
  const Protocol getProtocol(void) const;
  /** \brief gets reference url, if present.
   *  \return reference url to given service.
   */
  const ReferenceURL *getReferenceURL(void) const;

protected:
  /** \brief creates object from given data.
   *  \param name     service name.
   *  \param port     port number.
   *  \param protocol communication protocol.
   */
  Service(const Name      &name,
          const Port      &port,
          const Protocol  &protocol,
          ReferenceURLPtr  url);

private:
  Name            name_;
  Port            port_;
  Protocol        protocol_;
  ReferenceURLPtr url_;
}; // class Service


/** \brief smart pointer to service type.
 */
typedef boost::shared_ptr<Service> ServicePtr;

} // namespace Persistency

#endif
