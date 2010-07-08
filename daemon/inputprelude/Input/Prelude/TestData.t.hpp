/*
 * TestData.t.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_TESTDATA_T_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_TESTDATA_T_HPP_FILE

#include <prelude.h>

#include "System/ScopedPtrCustom.hpp"

namespace Input
{
namespace Prelude
{

// helpers to create prelude's data.
//
// NOTE: that each call returns pointer that is owned by this class (by
// proper member) - you can use pointer, but it will be deallocated
// automatically. it you need ownership call release() meothd on
// member object.
//
// NOTE: after calling to any member method value of all member fields, except
// for the one set (and returned) by this call, are undefined.
struct TestData
{
  idmef_message_t *makeMessage(void);
  idmef_analyzer_t *makeAnalyzerForAlert(idmef_alert_t *alert);
  idmef_analyzer_t *makeAnalyzerForHeartbeat(idmef_heartbeat_t *heartbeat);
  void fillAnalyzer(idmef_analyzer_t *analyzer,
                    const char       *id="Death/Star/ID/42",
                    const char       *name="Analyzer Of Luke Skywalker",
                    const char       *osType="Linux",
                    const char       *osVersion="2.6.129 gr-sec",
                    const char       *address="1.2.3.4");
  prelude_string_t *makeString(const char *str);
  idmef_alert_t *makeAlert(void);
  void makeAndFillNodeForAnalyzer(idmef_analyzer_t *analyzer,
                                  const char       *address);
  idmef_time_t *makeTime(time_t time);
  idmef_heartbeat_t *makeHeartbeat(void);

  System::ScopedPtrCustom<idmef_message_t,        idmef_message_destroy>        message_;
  System::ScopedPtrCustom<prelude_string_t,       prelude_string_destroy>       string_;
  System::ScopedPtrCustom<idmef_time_t,           idmef_time_destroy>           time_;
}; // struct TestData

} // namespace Prelude
} // namespace Input

#endif
