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
#include <boost/operators.hpp>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Persistency/ReferenceURL.hpp"
#include "Persistency/detail/LimitedString.hpp"
#include "Persistency/detail/LimitedNULLString.hpp"

namespace Persistency
{

/** \brief service's data representation.
 */
class Service: private boost::noncopyable,
               public  boost::equality_comparable<Service>
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

  /** \brief creates object from given data.
   *  \param name     service name.
   *  \param port     port number.
   *  \param protocol communication protocol.
   *  \param url      reference url of thie service.
   */
  Service(const Name      &name,
          const Port      &port,
          const Protocol  &protocol,
          ReferenceURLPtr  url);

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
  /** \brief check if classes are equal.
   *  \param other element to compare with.
   *  \return true if elements are equal, false otherwise.
   */
  bool operator==(const Service &other) const;

private:
  Name            name_;
  Port            port_;
  Protocol        protocol_;
  ReferenceURLPtr url_;
}; // class Service


/** \brief smart pointer to service type. */
typedef boost::shared_ptr<Service>         ServicePtr;

/** \brief smart pointer to service type, not NULL. */
typedef Commons::SharedPtrNotNULL<Service> ServicePtrNN;

} // namespace Persistency

#endif
