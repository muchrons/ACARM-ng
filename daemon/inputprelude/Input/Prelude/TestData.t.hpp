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
  idmef_analyzer_t *makeAnalyzerForHeartbeat(idmef_heartbeat_t *heartbeat);

  //Heartbeat
  idmef_heartbeat_t *makeHeartbeat(void);


  //Alert
  void makeAlert(void);
  void addClassificationToAlert(const char * classification);
  void addTimeToAlert(time_t time);

  idmef_analyzer_t *addAnalyzerToAlert();

  //Analyzer
  void addNameToAnalyzer(idmef_analyzer_t * analyzer, const char * name);
  void addAnalyzeridToAnalyzer(idmef_analyzer_t * analyzer, const char * value);
  void addOsTypeToAnalyzer(idmef_analyzer_t * analyzer, const char * value);
  void addOsVersionToAnalyzer(idmef_analyzer_t * analyzer, const char * value);
  void addAddressToAnalyzer(idmef_analyzer_t * analyzer, const char * address, bool v6=false);

  //Source
  idmef_source_t *addSourceToAlert();
  void addUserToSource(idmef_source_t * source, const char * user_name);
  void addProcessToSource(idmef_source_t * source, const char * process_name);
  void addServiceToSource(idmef_source_t * source, const char * service_name=NULL, const char * service_protocol=NULL, uint port=0);
  void addAddressToSource(idmef_source_t * analyzer, const char * address,bool v6);

  //Target
  idmef_target_t *addTargetToAlert();
  void addUserToTarget(idmef_target_t * target, const char * user_name);
  void addProcessToTarget(idmef_target_t * target, const char * process_name);
  void addServiceToTarget(idmef_target_t * target, const char * service_name=NULL, const char * service_protocol=NULL, uint port=0);
  void addAddressToTarget(idmef_target_t * analyzer, const char * address, bool v6);


  prelude_string_t *makeString(const char *str);
  idmef_time_t *makeTime(time_t time);
  void fillNode(idmef_node_t * node, const char * address, bool v6);
  void fillService(idmef_service_t * service, const char * service_name=NULL, const char * service_protocol=NULL, uint port=0);
  idmef_process_t * fillProcess(const char * process_name);
  void fillUser(idmef_user_t * user, const char * user_name);


  System::ScopedPtrCustom<idmef_message_t,        idmef_message_destroy>        message_;
  System::ScopedPtrCustom<prelude_string_t,       prelude_string_destroy>       string_;
  System::ScopedPtrCustom<idmef_time_t,           idmef_time_destroy>           time_;
}; // struct TestData

} // namespace Prelude
} // namespace Input

#endif
