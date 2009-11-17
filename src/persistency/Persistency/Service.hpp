/*
 * Service.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_SERVICE_HPP_FILE
#define INCLUDE_PERSISTENCY_SERVICE_HPP_FILE

/* public header */

#include <memory>
#include <stdint.h>
#include <boost/noncopyable.hpp>

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

protected:
  /** \brief creates object from given data.
   *  \param name     service name.
   *  \param port     port number.
   *  \param protocol communication protocol.
   */
  Service(const Name     &name,
          const Port     &port,
          const Protocol &protocol);

private:
  Name     name_;
  Port     port_;
  Protocol protocol_;
}; // class Service


/** \brief auto pointer to service type.
 */
typedef std::auto_ptr<Service> ServiceAutoPtr;

} // namespace Persistency

#endif
