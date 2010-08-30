/*
 * GeneralConfig.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_GENERALCONFIG_HPP_FILE
#define INCLUDE_CONFIGIO_GENERALCONFIG_HPP_FILE

/* public header */

#include <string>

#include "ConfigIO/ExceptionParseError.hpp"

namespace ConfigIO
{

/** \brief general configuration representation.
 *
 *  \note WUI is Web User Interface
 */
class GeneralConfig
{
public:
  /** \brief url representation. */
  typedef std::string  URL;
  /** \brief interval between two time periods. */
  typedef unsigned int Interval;

  /** \brief create object instance with given configuration.
   *  \param wuiUrl          address to WUI of ACARM-ng.
   *  \param cleanupInterval interval between persistency cleanups.
   */
  GeneralConfig(const URL &wuiUrl, Interval cleanupInterval);

  /** \brief gets WUI address.
   *  \return link to WUI.
   */
  const URL &getWUIUrl(void) const
  {
    return wuiUrl_;
  }
  /** \brief gets interval between cleanups of persistent storage.
   *  \return hours to elaps between cleanups.
   */
  Interval getCleanupInterval(void) const
  {
    return cleanupInterval_;
  }

private:
  URL      wuiUrl_;
  Interval cleanupInterval_;
}; // class GeneralConfig

} // namespace ConfigIO

#endif
