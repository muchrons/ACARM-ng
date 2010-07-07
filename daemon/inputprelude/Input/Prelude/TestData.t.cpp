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

idmef_analyzer_t *TestData::makeAnalyzerForAlert(idmef_alert_t *alert)
{
  if(alert==NULL)
    return NULL;
  idmef_analyzer_t *tmp=NULL;
  idmef_alert_new_analyzer(alert, &tmp, IDMEF_LIST_APPEND);
  return tmp;
}

idmef_analyzer_t *TestData::makeAnalyzerForHeartbeat(idmef_heartbeat_t *heartbeat)
{
  if(heartbeat==NULL)
    return NULL;
  idmef_analyzer_t *tmp=NULL;
  idmef_heartbeat_new_analyzer(heartbeat, &tmp, IDMEF_LIST_APPEND);
  return tmp;
}

void TestData::fillAnalyzer(idmef_analyzer_t *analyzer,
                            const char       *id,
                            const char       *name,
                            const char       *osType,
                            const char       *osVersion,
                            const char       *address)
{
  tut::ensure("analyzer cannot be NULL", analyzer!=NULL);

  idmef_analyzer_set_analyzerid(analyzer, makeString(id) );
  string_.release();
  idmef_analyzer_set_name(analyzer, makeString(name) );
  string_.release();
  idmef_analyzer_set_ostype(analyzer, makeString(osType) );
  string_.release();
  idmef_analyzer_set_osversion(analyzer, makeString(osVersion) );
  string_.release();

  makeAndFillNodeForAnalyzer(analyzer, address);
}

prelude_string_t *TestData::makeString(const char *str)
{
  if(str==NULL)
    return NULL;
  prelude_string_t *tmp=NULL;
  prelude_string_new_dup(&tmp, str);
  string_.reset(tmp);
  return tmp;
}

idmef_alert_t *TestData::makeAlert(void)
{
  idmef_message_t *message=makeMessage();
  idmef_alert_t   *tmp    =NULL;
  tut::ensure("unable to create alert", idmef_message_new_alert(message, &tmp)>=0 );
  //prelude_string_t *string1;
  //idmef_alert_new_messageid(alert,&string1);

  idmef_classification_t *classif=NULL;
  idmef_alert_new_classification(tmp, &classif);
  idmef_classification_set_text(classif, makeString("some classification") );
  string_.release();

  idmef_analyzer_t *analyzer=makeAnalyzerForAlert(tmp);
  fillAnalyzer(analyzer);

  return tmp;
}

void TestData::makeAndFillNodeForAnalyzer(idmef_analyzer_t *analyzer, const char *address)
{
  tut::ensure("analyzer cannot be NULL", analyzer!=NULL);
  idmef_node_t *node=NULL;
  idmef_analyzer_new_node(analyzer, &node);
  // fill node
  idmef_address_t *addr=NULL;
  idmef_node_new_address(node, &addr, IDMEF_LIST_APPEND);
  idmef_address_set_address(addr, makeString(address) );
  string_.release();
  idmef_address_set_category(addr, IDMEF_ADDRESS_CATEGORY_IPV4_ADDR);
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

  idmef_analyzer_t *analyzer=makeAnalyzerForHeartbeat(tmp);
  fillAnalyzer(analyzer);

  return tmp;
}

} // namespace Prelude
} // namespace Input
