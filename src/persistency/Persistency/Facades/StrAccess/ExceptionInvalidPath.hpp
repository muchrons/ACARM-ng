/*
 * ExceptionInvalidPath.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCES_EXCEPTIONINVALIDPATH_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCES_EXCEPTIONINVALIDPATH_HPP_FILE

/* public header */

#include "Persistency/Facades/StrAccess/Exception.hpp"

namespace Persistency
{
namespace Facades
{
namespace StrAccess
{

/** \brief exception throw when incorrect path has been detected.
 */
class ExceptionInvalidPath: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   *  \param path  full path that has been found invalid.
   *  \param near  place that couldn't be parsed correctly.
   */
  ExceptionInvalidPath(const Location &where, const std::string &path, const std::string &near);
}; // class ExceptionInvalidPath

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
