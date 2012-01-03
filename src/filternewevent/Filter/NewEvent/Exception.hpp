/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_FILTER_NEWEVENT_EXCEPTION_HPP_FILE
#define INCLUDE_FILTER_NEWEVENT_EXCEPTION_HPP_FILE

#include "Filter/Exception.hpp"

namespace Filter
{
namespace NewEvent
{
/** \brief base for all new-event-filter-related exceptions.
 */
class Exception: public Filter::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param what  message to represent.
   */
  Exception(const Location &where, const std::string &what):
    Filter::Exception(where, "newevent", what)
  {
  }
}; // class Exception

} // namespace NewEvent
} // namespace Filter

#endif
