/*
 * ExceptionInvalidPath.hpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_EXCEPTIONINVALIDPATH_HPP_FILE
#define INCLUDE_PREPROCESSOR_EXCEPTIONINVALIDPATH_HPP_FILE

/* public header */

#include "Preprocessor/Exception.hpp"

namespace Preprocessor
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
  ExceptionInvalidPath(const Location    &where,
                       const std::string &path,
                       const std::string &near):
    Exception(where, cc("invalid path '", path, "' near '", near, "' token") )
  {
  }
}; // class ExceptionInvalidPath

} // namespace Preprocessor

#endif
