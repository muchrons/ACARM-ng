/*
 * SanityCheck.hpp
 *
 */
#ifndef INCLUDE_CORE_SANITYCHECK_HPP_FILE
#define INCLUDE_CORE_SANITYCHECK_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>

#include "Core/Exception.hpp"
#include "Core/ExceptionRunningAsRoot.hpp"

namespace Core
{

/** \brief tests environment's sanity.
 *
 *  checks application's pre-conditions and throws in case of error.
 */
class SanityCheck: private boost::noncopyable
{
public:
  /** \brief perform checks
   */
  SanityCheck(void);

private:
  void ensureNotRoot(void);
}; // class SanityCheck

} // namespace Core

#endif
