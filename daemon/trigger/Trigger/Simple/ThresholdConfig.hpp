/*
 * ThresholdConfig.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_THRESHOLDCONFIG_HPP_FILE
#define INCLUDE_TRIGGER_THRESHOLDCONFIG_HPP_FILE

/* public header */

#include "Base/NullValue.hpp"
#include "Trigger/ExceptionInvalidValue.hpp"

namespace Trigger
{
namespace Simple
{

/** \brief configuration for threshold values.
 */
class ThresholdConfig
{
public:
  /** \brief object representing severity threshold. */
  typedef Base::NullValue<double> SeverityThreshold;
  /** \brief object representing alerts' count threshold. */
  typedef Base::NullValue<size_t> AlertCountThreshold;

  /** \brief parse strings representing threshold values and save them for further usage.
   *  \param severity string represenitng severity threshold (NULL if not set).
   *  \param count    string represenitng count threshold (NULL if not set).
   */
  ThresholdConfig(const char *severity, const char *count);

  /** \brief gets severity threshold.
   *  \return threshold value (may be NULL).
   */
  SeverityThreshold getSeverityThreshold(void) const
  {
    return severity_;
  }
  /** \brief gets alerts count threshold.
   *  \return threshold value (may be NULL).
   */
  AlertCountThreshold getAlertCountThreshold(void) const
  {
    return cnt_;
  }

private:
  SeverityThreshold   severity_;
  AlertCountThreshold cnt_;
}; // class ThresholdConfig

} // namespace Simple
} // namespace Trigger

#endif
