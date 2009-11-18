/*
 * Service.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_STUBS_SERVICE_HPP_FILE
#define INCLUDE_PERSISTENCY_STUBS_SERVICE_HPP_FILE

/* public header */

#include "Persistency/Service.hpp"
#include "Persistency/Stubs/ReferenceURL.hpp"

namespace Persistency
{
namespace Stubs
{
/** \brief service's data representation.
 */
class Service: public Persistency::Service
{
  /** \brief creates object from given data.
   *  \param name     service name.
   *  \param port     port number.
   *  \param protocol communication protocol.
   *  \param url      reference url of thie service.
   */
  Service(const Name      &name,
          const Port      &port,
          const Protocol  &protocol,
          ReferenceURLPtr  url):
    Persistency::Service(name, port, protocol, url)
  {
  }
  /** \brief create sample service.
   */
  Service(void):
    Persistency::Service("service 1", 42, "proto1",
                         ReferenceURLPtr(new ReferenceURL) )
  {
  }
}; // class Service

} // namespace Stubs
} // namespace Persistency

#endif
