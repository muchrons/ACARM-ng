/*
 * PortNumber.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_PORTNUMBER_HPP_FILE
#define INCLUDE_PERSISTENCY_PORTNUMBER_HPP_FILE

/* public header */

#include <stdint.h>
#include <boost/operators.hpp>

#include "Persistency/Exception.hpp"

namespace Persistency
{

/** \brief representation of port number
 */
class PortNumber: public boost::equality_comparable<PortNumber>
{
public:
  struct ExceptionInvalidPort: public Exception
  {
    /** \brief create execption with given message.
     *  \param where place where exception has been thrown.
     *  \param port  port number that has been found invalid.
     */
    ExceptionInvalidPort(const Location &where, uint16_t port):
        Exception(where, cc("ginve port number is invalid: ", port) )
    {
    }
  }; // struct ExceptionInvalidPort

  /** \brief create object.
   *  \param port port number to represent.
   */
  PortNumber(const uint16_t port):
    port_(port)
  {
    if(port_==0)
      throw ExceptionInvalidPort(SYSTEM_SAVE_LOCATION, port_);
  }

  /** \brief gets saved port number.
   *  \return port number.
   */
  uint16_t get(void) const
  {
    return port_;
  }

private:
  uint16_t port_;
}; // class PortNumber

} // namespace Persistency

#endif
