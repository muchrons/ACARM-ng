/*
 * ExceptionNetworkError.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SNORTSAM_EXCEPTIONNETWORKERROR_HPP_FILE
#define INCLUDE_TRIGGER_SNORTSAM_EXCEPTIONNETWORKERROR_HPP_FILE

#include "Trigger/SnortSam/Exception.hpp"

namespace Trigger
{
namespace SnortSam
{

/** \brief exception thrown when there is network-related problem spotted.
 */
class ExceptionNetworkError: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where   place where exception has been thrown.
   *  \param host    IP or host name of the other side of the channel.
   *  \param details details on problem spotted.
   */
  ExceptionNetworkError(const Location    &where,
                        const std::string &host,
                        const std::string &details);
}; // class ExceptionNetworkError

} // namespace SnortSam
} // namespace Trigger

#endif
