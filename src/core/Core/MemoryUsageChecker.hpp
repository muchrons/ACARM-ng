/*
 * MemoryUsageChecker.hpp
 *
 */
#ifndef INCLUDE_CORE_MEMORYUSAGECHECKER_HPP_FILE
#define INCLUDE_CORE_MEMORYUSAGECHECKER_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>
#include "Logger/Node.hpp"

namespace Core
{

/** \brief helper object that checks if we are running out of memory.
 */
class MemoryUsageChecker: private boost::noncopyable
{
public:
  /** \brief Create object that checks OOMs.
   */
  MemoryUsageChecker(void);
  /** \brief Checks if we are short of memory.
   */
  bool iSmemoryLimitExceeded() const;

private:
  Logger::Node log_;
  unsigned int maxMem_;
  mutable clock_t lastNotification_;
}; // class MemoryUsageChecker

} // namespace Core

#endif
