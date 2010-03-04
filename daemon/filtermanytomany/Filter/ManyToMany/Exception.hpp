/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_FILTER_MANYTOMANY_EXCEPTION_HPP_FILE
#define INCLUDE_FILTER_MANYTOMANY_EXCEPTION_HPP_FILE

/* public header */

#include "Filter/Exception.hpp"

// TODO: is this needed?

namespace Filter
{
namespace ManyToMany
{
/** \brief base for exception for filters looking for host-match
 */
class Exception: public Filter::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param name  name of this filter.
   *  \param msg   message to represent.
   */
  Exception(const Location &where, const char *name, const char *msg):
    Filter::Exception(where, name, msg)
  {
  }
}; // class Exception

} // namespace ManyToMany
} // namespace Filter

#endif
