/*
 * PersistencyCleanup.hpp
 *
 */
#ifndef INCLUDE_CORE_PERSISTENCYCLEANUP_HPP_FILE
#define INCLUDE_CORE_PERSISTENCYCLEANUP_HPP_FILE

/* public header */

#include "Logger/Node.hpp"
#include "ConfigIO/GeneralConfig.hpp"

namespace Core
{

/** \brief class for cleaning up old entries from persistent storage.
 */
class PersistencyCleanup
{
public:
  /** \brief creates instance.
   */
  explicit PersistencyCleanup(ConfigIO::GeneralConfig::Timespan cleanupOlder);
  /** \brief run cleanup manually.
   */
  void cleanup(void);

private:
  Logger::Node                      log_;
  ConfigIO::GeneralConfig::Timespan cleanupOlder_;
}; // class PersistencyCleanup

} // namespace Core

#endif
