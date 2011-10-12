/*
 * ExceptionCryptoFailed.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SNORTSAM_EXCEPTIONCRYPTOFAILED_HPP_FILE
#define INCLUDE_TRIGGER_SNORTSAM_EXCEPTIONCRYPTOFAILED_HPP_FILE

#include "Trigger/SnortSam/Exception.hpp"

namespace Trigger
{
namespace SnortSam
{

/** \brief exception thrown cryptographic function failed.
 */
class ExceptionCryptoFailed: public Exception
{
public:
  /** \brief create execption with given message.
   *  \param where   place where exception has been thrown.
   *  \param details details on problem spotted.
   */
  ExceptionCryptoFailed(const Location    &where,
                        const std::string &details);
}; // class ExceptionCryptoFailed

} // namespace SnortSam
} // namespace Trigger

#endif
