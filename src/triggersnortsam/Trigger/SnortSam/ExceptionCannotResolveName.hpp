/*
 * ExceptionCannotResolveName.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SNORTSAM_EXCEPTIONCANNOTRESOLVENAME_HPP_FILE
#define INCLUDE_TRIGGER_SNORTSAM_EXCEPTIONCANNOTRESOLVENAME_HPP_FILE

#include "Trigger/SnortSam/Exception.hpp"

namespace Trigger
{
namespace SnortSam
{

/** \brief exception thrown when hostname cannot be reolved
 */
class ExceptionCannotResolveName: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where   place where exception has been thrown.
   *  \param host    name of the host to be resolved.
   *  \param details details on problem spotted.
   */
  ExceptionCannotResolveName(const Location    &where,
                             const std::string &host,
                             const std::string &details);
}; // class ExceptionCannotResolveName

} // namespace SnortSam
} // namespace Trigger

#endif
