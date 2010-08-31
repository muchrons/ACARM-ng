/*
 * FilterConfig.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_FILTERCONFIG_HPP_FILE
#define INCLUDE_CONFIGIO_FILTERCONFIG_HPP_FILE

/* public header */

#include <vector>

#include "ConfigIO/Generic/Config.hpp"

namespace ConfigIO
{

/** \brief class representing configuration of filter.
 */
class FilterConfig: public Generic::Config<FilterConfig>
{
public:
  /** \brief creates filter's configuration.
   *  \param type    filter's type.
   *  \param options options for given filter.
   */
  FilterConfig(const TypeName &type,
               const Options  &options):
    Generic::Config<FilterConfig>(type, options)
  {
  }
  /** \brief gets filter's name.
   *  \return filter's name.
   */
  std::string getFilterName(void)
  {
    return get("name");
  }
}; // class FilterConfig

} // namespace ConfigIO

#endif
