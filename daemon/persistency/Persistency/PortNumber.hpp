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
class PortNumber: public boost::less_than_comparable<PortNumber>,
                  public boost::equivalent<PortNumber>,
                  public boost::equality_comparable<PortNumber>
{
public:
  /** \brief exception thrown when invalid port number is provided.
   */
  struct ExceptionInvalidPort: public Exception
  {
    /** \brief create execption with given message.
     *  \param where place where exception has been thrown.
     *  \param port  port number that has been found invalid.
     */
    ExceptionInvalidPort(const Location &where, uint16_t port):
        Exception(where, cc("given port number is invalid: ", port) )
    {
    }
  }; // struct ExceptionInvalidPort

  /** \brief create object.
   *  \param port port number to represent.
   */
  PortNumber(const uint16_t port):
    port_(port)
  {
    if(port_==0u)
      throw ExceptionInvalidPort(SYSTEM_SAVE_LOCATION, port_);
  }

  /** \brief gets saved port number.
   *  \return port number.
   */
  uint16_t get(void) const
  {
    return port_;
  }

  /** \brief less-than comparison.
   *  \param other element to compare with
   *  \return true if this object's port number is less than other's, false otherwise.
   */
  bool operator<(const PortNumber &other) const
  {
    return get() < other.get();
  }

private:
  uint16_t port_;
}; // class PortNumber

} // namespace Persistency

#endif
