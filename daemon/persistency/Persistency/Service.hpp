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
#include "Commons/LimitedString.hpp"
#include "Commons/LimitedNULLString.hpp"
#include "Persistency/ReferenceURL.hpp"
#include "Persistency/PortNumber.hpp"

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
  typedef Commons::LimitedString<32>     Name;
  /** \brief protocol used by sevice.
   */
  typedef Commons::LimitedNULLString<32> Protocol;

  /** \brief creates object from given data.
   *  \param name     service name.
   *  \param port     port number.
   *  \param protocol communication protocol.
   *  \param url      reference url of thie service.
   */
  Service(const Name       &name,
          const PortNumber &port,
          const Protocol   &protocol,
          ReferenceURLPtr   url);

  /** \brief gets service name.
   *  \return name of the service.
   */
  const Name &getName(void) const;
  /** \brief get port number.
   *  \return port number service is running on.
   */
  PortNumber getPort(void) const;
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
  PortNumber      port_;
  Protocol        protocol_;
  ReferenceURLPtr url_;
}; // class Service


/** \brief smart pointer to service type. */
typedef boost::shared_ptr<Service>                                  ServicePtr;

/** \brief smart pointer to service type. */
typedef boost::shared_ptr<const ServicePtr::element_type>           ConstServicePtr;

/** \brief smart pointer to service type, not NULL. */
typedef Commons::SharedPtrNotNULL<ServicePtr::element_type>         ServicePtrNN;

/** \brief smart pointer to service type, not NULL. */
typedef Commons::SharedPtrNotNULL<const ServicePtrNN::element_type> ConstServicePtrNN;

} // namespace Persistency

#endif
