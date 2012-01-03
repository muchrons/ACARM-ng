/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_FILTER_DNSRESOLVER_EXCEPTION_HPP_FILE
#define INCLUDE_FILTER_DNSRESOLVER_EXCEPTION_HPP_FILE

#include "Filter/Exception.hpp"

namespace Filter
{
namespace DNSResolver
{

/** \brief base exception for DNS-resolver filter.
 */
class Exception: public Filter::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param msg   message to represent.
   */
  Exception(const Location &where, const std::string &msg):
    Filter::Exception(where, "dnsresolver", msg)
  {
  }
}; // class Exception

} // namespace DNSResolver
} // namespace Filter

#endif
