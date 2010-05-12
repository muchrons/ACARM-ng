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

// TODO: comment

/** \brief configuration for threshold values.
 */
class ThresholdConfig
{
public:
  typedef Base::NullValue<double> SeverityThreshold;
  typedef Base::NullValue<size_t> AlertCountThreshold;

  ThresholdConfig(const char *severity, const char *count);

  SeverityThreshold getSeverityThreshold(void) const
  {
    return severity_;
  }

  AlertCountThreshold getAlertCountThreshold(void) const
  {
    return cnt_;
  }

private:
  SeverityThreshold   severity_;
  AlertCountThreshold cnt_;
}; // class ThresholdConfig

} // namespace Trigger

#endif
