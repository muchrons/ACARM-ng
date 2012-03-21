/*
 * PersistencyPeriodicQueries.hpp
 *
 */
#ifndef INCLUDE_CORE_PERSISTENCYPERIODICQUERIES_HPP_FILE
#define INCLUDE_CORE_PERSISTENCYPERIODICQUERIES_HPP_FILE

/* public header */

#include "Logger/Node.hpp"
#include "ConfigIO/GeneralConfig.hpp"

namespace Core
{

/** \brief class for periodically issuing commands on persistent storage
 */
class PersistencyPeriodicQueries
{
public:
  /** \brief creates instance.
   */
  PersistencyPeriodicQueries();
  /** \brief runs queries manually.
   */
  void issue(void);

private:
  Logger::Node                      log_;
}; // class PersistencyAuxiliarytables

} // namespace Core

#endif
