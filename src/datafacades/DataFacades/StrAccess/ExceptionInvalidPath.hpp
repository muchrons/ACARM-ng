/*
 * ExceptionInvalidPath.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_STRACCES_EXCEPTIONINVALIDPATH_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_STRACCES_EXCEPTIONINVALIDPATH_HPP_FILE

/* public header */

#include "DataFacades/StrAccess/Path.hpp"
#include "DataFacades/StrAccess/Exception.hpp"

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
   *  \param where   place where exception has been thrown.
   *  \param path    full path that has been found invalid.
   *  \param near    place that couldn't be parsed correctly.
   *  \param details more details on error situation.
   */
  ExceptionInvalidPath(const Location    &where,
                       const std::string &path,
                       const std::string &near,
                       const std::string &details);
}; // class ExceptionInvalidPath

} // namespace StrAccess
} // namespace Facades
} // namespace Persistency

#endif
