/*
 * PersistencyConfig.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_PERSISTENCYCONFIG_HPP_FILE
#define INCLUDE_CONFIGIO_PERSISTENCYCONFIG_HPP_FILE

/* public header */

#include "ConfigIO/Generic/Config.hpp"

namespace ConfigIO
{

/** \brief class representing configuration of persistency storage.
 */
class PersistencyConfig: public Generic::Config<PersistencyConfig>
{
public:
  /** \brief creates persistency configuration.
   *  \param type    persistency type.
   *  \param options options for persistency.
   */
  PersistencyConfig(const TypeName &type,
                    const Options  &options):
    Generic::Config<PersistencyConfig>(type, options)
  {
  }
}; // class PersistencyConfig

} // namespace ConfigIO

#endif
