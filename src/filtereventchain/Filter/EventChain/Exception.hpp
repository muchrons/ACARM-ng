/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_FILTER_EVENTCHAIN_EXCEPTION_HPP_FILE
#define INCLUDE_FILTER_EVENTCHAIN_EXCEPTION_HPP_FILE

#include "Filter/Exception.hpp"

namespace Filter
{
namespace EventChain
{
/** \brief base for all event-chain-filter-related exceptions.
 */
class Exception: public Filter::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param what  message to represent.
   */
  Exception(const Location &where, const std::string &what):
    Filter::Exception(where, "eventchain", what)
  {
  }
}; // class Exception

} // namespace EventChain
} // namespace Filter

#endif
