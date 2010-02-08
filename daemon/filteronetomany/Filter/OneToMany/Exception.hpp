/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_FILTER_ONETOMANY_EXCEPTION_HPP_FILE
#define INCLUDE_FILTER_ONETOMANY_EXCEPTION_HPP_FILE

/* public header */

// TODO

#include "Filter/Exception.hpp"

namespace Filter
{
namespace OneToMany
{
/** \brief base ofr OneToMany-filter exceptions
 */
class Exception: public Filter::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param msg   message to represent.
   */
  Exception(const Location &where, const char *msg):
    Filter::Exception(where, "onetomany", msg)
  {
  }
}; // class Exception

} // namespace OneToMany
} // namespace Filter

#endif
