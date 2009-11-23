/*
 * Service.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_POSTGRES_SERVICE_HPP_FILE
#define INCLUDE_PERSISTENCY_POSTGRES_SERVICE_HPP_FILE

#include "Persistency/Service.hpp"

namespace Persistency
{
namespace Postgres
{
/** \brief service's data representation.
 */
class Service: public Persistency::Service
{
public:
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
}; // class Service

} // namespace Postgres
} // namespace Persistency

#endif
