/*
 * GeneralConfig.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_GENERALCONFIG_HPP_FILE
#define INCLUDE_CONFIGIO_GENERALCONFIG_HPP_FILE

/* public header */

#include <string>

#include "ConfigIO/ExceptionInvalidValue.hpp"

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
  // TODO: consider making these two typedefs one
  /** \brief interval between two time periods. */
  typedef unsigned int Interval;
  /** \brief represents osme time length. */
  typedef unsigned int Timespan;

  /** \brief create object instance with given configuration.
   *  \param wuiUrl          address to WUI of ACARM-ng.
   *  \param cleanupInterval interval between persistency cleanups.
   *  \param cleanupOlder    maximum age for alerts before removal (0 means never remove).
   */
  GeneralConfig(const URL &wuiUrl, Interval cleanupInterval, Timespan cleanupOlder);

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
  /** \brief gets  maximum age for alert before it is wiped out.
   *  \return days to keep alerts in DB.
   */
  Timespan getCleanupOlder(void) const
  {
    return cleanupOlder_;
  }

private:
  URL      wuiUrl_;
  Interval cleanupInterval_;
  Timespan cleanupOlder_;
}; // class GeneralConfig

} // namespace ConfigIO

#endif
