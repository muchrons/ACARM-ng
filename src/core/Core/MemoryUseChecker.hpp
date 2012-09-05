/*
 * MemoryUsageChecker.hpp
 *
 */
#ifndef INCLUDE_CORE_MEMORYUSAGECHECKER_HPP_FILE
#define INCLUDE_CORE_MEMORYUSAGECHECKER_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>
#include "Logger/Node.hpp"
#include "Core/Types/SignedNodesFifo.hpp"

namespace Core
{

/** \brief helper object that checks if we are running out of memory.
 */
class MemoryUseChecker: private boost::noncopyable
{
public:
  /** \brief Create object that checks OOMs.
   */
  MemoryUseChecker(const Core::Types::SignedNodesFifo & queue);

  /** \brief Checks if we are short of memory.
   */
  bool alertsLimitExceeded() const;

private:
  Logger::Node log_;
  const Core::Types::SignedNodesFifo & queue_;
  size_t maxAlerts_;
}; // class MemoryUsageChecker

} // namespace Core

#endif
