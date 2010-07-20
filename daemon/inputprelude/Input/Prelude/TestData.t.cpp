/*
 * TestData.t.cpp
 *
 */
#include <tut.h>

#include "Input/Prelude/TestData.t.hpp"

namespace Input
{
namespace Prelude
{

idmef_message_t *TestData::makeMessage(void)
{
  idmef_message_t *tmp=NULL;
  tut::ensure("unable to create message", idmef_message_new(&tmp)>=0 );
  message_.reset(tmp);
  return tmp;
}

void TestData::makeAlert(void)
{
  makeMessage();
  idmef_alert_t   *tmp    =NULL;
  tut::ensure("unable to create alert", idmef_message_new_alert(message_.get(), &tmp)>=0 );
}

void TestData::addClassificationToAlert(const char * text)
{
  idmef_alert_t *alert=idmef_message_get_alert(message_.get());
  idmef_classification_t *classif=NULL;
  idmef_alert_new_classification(alert, &classif);
  idmef_classification_set_text(classif, makeString(text) );
}


idmef_analyzer_t *TestData::addAnalyzerToAlert()
{
  idmef_alert_t *alert=idmef_message_get_alert(message_.get());
  tut::ensure("alert not created", alert!=NULL );
  idmef_analyzer_t *tmp=NULL;
  idmef_alert_new_analyzer(alert, &tmp, IDMEF_LIST_APPEND);
  return tmp;
}

void TestData::addTimeToAlert(time_t time)
{
  idmef_alert_t *alert=idmef_message_get_alert(message_.get());
  tut::ensure("alert not created", alert!=NULL );
  idmef_alert_set_create_time(alert, makeTime(time));
  time_.release();
}

idmef_analyzer_t *TestData::makeAnalyzerForHeartbeat(idmef_heartbeat_t *heartbeat)
{
  idmef_heartbeat_t *alert=idmef_message_get_heartbeat(message_.get());
  tut::ensure("heartbeat not created", alert!=NULL );
  idmef_analyzer_t *tmp=NULL;
  idmef_heartbeat_new_analyzer(heartbeat, &tmp, IDMEF_LIST_APPEND);
  return tmp;
}

void TestData::addNameToAnalyzer(idmef_analyzer_t * analyzer, const char * value)
{
  tut::ensure("analyzer cannot be NULL", analyzer!=NULL);
  idmef_analyzer_set_name(analyzer, makeString(value) );
}

void TestData::addAnalyzeridToAnalyzer(idmef_analyzer_t * analyzer, const char * value)
{
  tut::ensure("analyzer cannot be NULL", analyzer!=NULL);
  idmef_analyzer_set_analyzerid(analyzer, makeString(value) );
}

void TestData::addOsTypeToAnalyzer(idmef_analyzer_t * analyzer, const char * value)
{
  tut::ensure("analyzer cannot be NULL", analyzer!=NULL);
  idmef_analyzer_set_ostype(analyzer, makeString(value) );
}

void TestData::addOsVersionToAnalyzer(idmef_analyzer_t * analyzer, const char * value)
{
  tut::ensure("analyzer cannot be NULL", analyzer!=NULL);
  idmef_analyzer_set_osversion(analyzer, makeString(value) );
}

void TestData::addAddressv4ToAnalyzer(idmef_analyzer_t * analyzer, const char * address)
{
  tut::ensure("analyzer cannot be NULL", analyzer!=NULL);
  idmef_node_t *node=NULL;
  idmef_analyzer_new_node(analyzer, &node);
  // fill node
  idmef_address_t *addr=NULL;
  idmef_node_new_address(node, &addr, IDMEF_LIST_APPEND);
  idmef_address_set_address(addr, makeString(address) );
  idmef_address_set_category(addr, IDMEF_ADDRESS_CATEGORY_IPV4_ADDR);
}

void TestData::addAddressv6ToAnalyzer(idmef_analyzer_t * analyzer, const char * address)
{
  tut::ensure("analyzer cannot be NULL", analyzer!=NULL);
  idmef_node_t *node=NULL;
  idmef_analyzer_new_node(analyzer, &node);
  // fill node
  idmef_address_t *addr=NULL;
  idmef_node_new_address(node, &addr, IDMEF_LIST_APPEND);
  idmef_address_set_address(addr, makeString(address) );
  idmef_address_set_category(addr, IDMEF_ADDRESS_CATEGORY_IPV6_ADDR);
}


prelude_string_t *TestData::makeString(const char *str)
{
  if(str==NULL)
    return NULL;
  prelude_string_t *tmp=NULL;
  prelude_string_new_dup(&tmp, str);
  return tmp;
}

idmef_time_t *TestData::makeTime(time_t time)
{
  idmef_time_t  *tmp=NULL;
  idmef_time_new_from_time(&tmp, &time);
  time_.reset(tmp);
  return tmp;
}

idmef_heartbeat_t *TestData::makeHeartbeat(void)
{
  idmef_message_t   *message=makeMessage();
  idmef_heartbeat_t *tmp    =NULL;
  tut::ensure("unable to create heartbeat", idmef_message_new_heartbeat(message, &tmp)>=0 );

  //idmef_analyzer_t *analyzer=makeAnalyzerForHeartbeat(tmp);
  //fillAnalyzer(analyzer);

  return tmp;
}

idmef_source_t * TestData::addSourceToAlert()
{
  idmef_alert_t *alert=idmef_message_get_alert(message_.get());
  tut::ensure("alert not created", alert!=NULL );
  idmef_source_t *source;
  if (idmef_alert_new_source(alert,(idmef_source_t **) &source,IDMEF_LIST_APPEND)<0)
    tut::fail("Unable to create source obcject.");
  return source;
}

void TestData::addUserToSource(idmef_source_t * source, const char * user_name)
{
  idmef_user_t *user;
  idmef_source_new_user(source,&user);

  idmef_user_id_t* userid;
  idmef_user_new_user_id(user,&userid,IDMEF_LIST_APPEND);

  prelude_string_t *username;
  prelude_string_new_dup(&username,user_name);
  idmef_user_id_set_name(userid,username);
}

void TestData::addProcessToSource(idmef_source_t * source, const char * process_name)
{
  prelude_string_t *proc_str;
  prelude_string_new_dup(&proc_str,process_name);

  idmef_process_t *proc;
  idmef_process_new(&proc);
  idmef_process_set_name(proc,proc_str);
  idmef_source_set_process(source,proc);
}


void TestData::addServiceToSource(idmef_source_t * source, const char * service_name, const char * service_protocol, uint port)
{
  idmef_service_t * service;
  idmef_source_new_service(source,&service);

  if (service_name!=NULL)
    {
      prelude_string_t *servicename;
      prelude_string_new_dup(&servicename,service_name);
      idmef_service_set_name(service,servicename);
    }

  if (service_protocol!=NULL)
    {
      prelude_string_t *protocol;
      prelude_string_new_dup(&protocol,service_protocol);
      idmef_service_set_protocol(service,protocol);
    }

  if (port!=0)
    idmef_service_set_port(service,port);
}

void TestData::addAddressv4ToSource(idmef_source_t * source, const char * address)
{
  tut::ensure("source cannot be NULL", source!=NULL);
  idmef_node_t *node=NULL;
  idmef_source_new_node(source, &node);
  // fill node
  idmef_address_t *addr=NULL;
  idmef_node_new_address(node, &addr, IDMEF_LIST_APPEND);
  idmef_address_set_address(addr, makeString(address) );
  idmef_address_set_category(addr, IDMEF_ADDRESS_CATEGORY_IPV4_ADDR);
}

void TestData::addAddressv6ToSource(idmef_source_t * source, const char * address)
{
  tut::ensure("source cannot be NULL", source!=NULL);
  idmef_node_t *node=NULL;
  idmef_source_new_node(source, &node);
  // fill node
  idmef_address_t *addr=NULL;
  idmef_node_new_address(node, &addr, IDMEF_LIST_APPEND);
  idmef_address_set_address(addr, makeString(address) );
  idmef_address_set_category(addr, IDMEF_ADDRESS_CATEGORY_IPV6_ADDR);
}


idmef_target_t * TestData::addTargetToAlert()
{
  idmef_alert_t *alert=idmef_message_get_alert(message_.get());
  tut::ensure("alert not created", alert!=NULL );
  idmef_target_t *target;
  if (idmef_alert_new_target(alert,(idmef_target_t **) &target,IDMEF_LIST_APPEND)<0)
    tut::fail("Unable to create target obcject.");
  return target;
}

void TestData::addUserToTarget(idmef_target_t * target, const char * user_name)
{
  idmef_user_t *user;
  idmef_target_new_user(target,&user);

  idmef_user_id_t* userid;
  idmef_user_new_user_id(user,&userid,IDMEF_LIST_APPEND);

  prelude_string_t *username;
  prelude_string_new_dup(&username,user_name);
  idmef_user_id_set_name(userid,username);
}

void TestData::addProcessToTarget(idmef_target_t * target, const char * process_name)
{
  prelude_string_t *proc_str;
  prelude_string_new_dup(&proc_str,process_name);

  idmef_process_t *proc;
  idmef_process_new(&proc);
  idmef_process_set_name(proc,proc_str);
  idmef_target_set_process(target,proc);
}


void TestData::addServiceToTarget(idmef_target_t * target, const char * service_name, const char * service_protocol, uint port)
{
  idmef_service_t * service;
  idmef_target_new_service(target,&service);

  if (service_name!=NULL)
    {
      prelude_string_t *servicename;
      prelude_string_new_dup(&servicename,service_name);
      idmef_service_set_name(service,servicename);
    }

  if (service_protocol!=NULL)
    {
      prelude_string_t *protocol;
      prelude_string_new_dup(&protocol,service_protocol);
      idmef_service_set_protocol(service,protocol);
    }

  if (port!=0)
    idmef_service_set_port(service,port);
}

void TestData::addAddressv4ToTarget(idmef_target_t * target, const char * address)
{
  tut::ensure("target cannot be NULL", target!=NULL);
  idmef_node_t *node=NULL;
  idmef_target_new_node(target, &node);
  // fill node
  idmef_address_t *addr=NULL;
  idmef_node_new_address(node, &addr, IDMEF_LIST_APPEND);
  idmef_address_set_address(addr, makeString(address) );
  idmef_address_set_category(addr, IDMEF_ADDRESS_CATEGORY_IPV4_ADDR);
}

void TestData::addAddressv6ToTarget(idmef_target_t * target, const char * address)
{
  tut::ensure("target cannot be NULL", target!=NULL);
  idmef_node_t *node=NULL;
  idmef_target_new_node(target, &node);
  // fill node
  idmef_address_t *addr=NULL;
  idmef_node_new_address(node, &addr, IDMEF_LIST_APPEND);
  idmef_address_set_address(addr, makeString(address) );
  idmef_address_set_category(addr, IDMEF_ADDRESS_CATEGORY_IPV6_ADDR);
}


} // namespace Prelude
} // namespace Input
