/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_FILTER_MANYTOMANY_EXCEPTION_HPP_FILE
#define INCLUDE_FILTER_MANYTOMANY_EXCEPTION_HPP_FILE

#include "Filter/Exception.hpp"

namespace Filter
{
namespace ManyToMany
{
/** \brief base for all many-to-many-filter-related exceptions.
 */
class Exception: public Filter::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param what  message to represent.
   */
  Exception(const Location &where, const std::string &what):
    Filter::Exception(where, "manytomany", what)
  {
  }
}; // class Exception

} // namespace ManyToMany
} // namespace Filter

#endif
