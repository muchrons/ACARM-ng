/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FILESYSTEM_EXCEPTION_HPP_FILE
#define INCLUDE_COMMONS_FILESYSTEM_EXCEPTION_HPP_FILE

/* public header */

#include "Commons/Exception.hpp"

namespace Commons
{
namespace Filesystem
{

/** \brief exception thrown when silesystem-related operation failed.
 */
class Exception: public Commons::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been rised.
   *  \param what  details on problem spotted.
   */
  Exception(const Location &where, const std::string &what);
}; // class Exception

} // namespace Filesystem
} // namespace Commons

#endif
