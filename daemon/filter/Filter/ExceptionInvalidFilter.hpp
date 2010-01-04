/*
 * ExceptionInvalidFilter.hpp
 *
 */
#ifndef INCLUDE_FILTER_EXCEPTIONINVALIDFILTER_HPP_FILE
#define INCLUDE_FILTER_EXCEPTIONINVALIDFILTER_HPP_FILE

/* public header */

#include <string>

#include "Filter/Exception.hpp"

namespace Filter
{
/** \brief exception thrown when invalid filter is used.
 */
class ExceptionInvalidFilter: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param name  name of invalid filter.
   */
  ExceptionInvalidFilter(const char *where, const char *name):
    Exception(where, std::string("invalid filter: ")+ensureString(name) )
  {
  }
}; // class ExceptionInvalidFilter

} // namespace Filter

#endif
