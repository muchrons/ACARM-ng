/*
 * PersistencyCleanup.hpp
 *
 */
#ifndef INCLUDE_CORE_PERSISTENCYCLEANUP_HPP_FILE
#define INCLUDE_CORE_PERSISTENCYCLEANUP_HPP_FILE

/* public header */

#include "Logger/Node.hpp"

namespace Core
{

/** \brief class for cleaning up old entries from persistent storage.
 */
class PersistencyCleanup
{
public:
  /** \brief creates instance.
   *
   *  when creating cleanup run by default.
   */
  PersistencyCleanup(void);
  /** \brief run cleanup manually.
   */
  void cleanup(void);

private:
  Logger::Node log_;
}; // class PersistencyCleanup

} // namespace Core

#endif
