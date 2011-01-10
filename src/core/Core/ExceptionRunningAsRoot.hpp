/*
 * Exception.hpp
 *
 */
#ifndef INCLUDE_CORE_EXCEPTIONRUNNINGASROOT_HPP_FILE
#define INCLUDE_CORE_EXCEPTIONRUNNINGASROOT_HPP_FILE

/* public header */

#include "Commons/Exception.hpp"

namespace Core
{
/** \brief exception thrown when system is being run as root.
 */
class ExceptionRunningAsRoot: public Commons::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where place where exception has been thrown.
   */
  explicit ExceptionRunningAsRoot(const Location &where):
    Commons::Exception(where, "trying to run system as root! this is __VERY__ dangereous "
                              "thus not allowed - run system as non-privilaged used. "
                              "NOTE: it is recommend to run system from a separate, dedicated account.")
  {
  }
}; // class ExceptionRunningAsRoot

} // namespace Core

#endif
