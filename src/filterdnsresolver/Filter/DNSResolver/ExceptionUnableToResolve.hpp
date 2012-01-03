/*
 * ExceptionUnableToResolve.hpp
 *
 */
#ifndef INCLUDE_FILTER_DNSRESOLVER_EXCEPTIONUNABLETORESOLVE_HPP_FILE
#define INCLUDE_FILTER_DNSRESOLVER_EXCEPTIONUNABLETORESOLVE_HPP_FILE

#include "Filter/DNSResolver/Exception.hpp"

namespace Filter
{
namespace DNSResolver
{

/** \brief exception thrown when DNS fails.
 */
class ExceptionUnableToResolve: public DNSResolver::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param ip    ip that was not resolved.
   *  \param msg   problem description.
   */
  ExceptionUnableToResolve(const Location &where, const std::string &ip, const std::string &msg):
    DNSResolver::Exception(where, cc("error resolving host '", ip, "': ", msg).c_str() )
  {
  }
}; // class ExceptionUnableToResolve

} // namespace DNSResolver
} // namespace Filter

#endif
