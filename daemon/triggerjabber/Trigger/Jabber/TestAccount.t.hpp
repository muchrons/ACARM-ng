/*
 * TestAccount.t.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_JABBER_TESTACCOUNT_T_HPP_FILE
#define INCLUDE_TRIGGER_JABBER_TESTACCOUNT_T_HPP_FILE

#include <glib.h>
#include <boost/algorithm/string.hpp>
using namespace boost;

#include "System/ScopedPtrCustom.hpp"
#include "Trigger/Jabber/AccountConfig.hpp"
#include "Trigger/Jabber/Connection.hpp"
#include "TestHelpers/Data/jabber1.hpp"
#include "TestHelpers/Data/jabber2.hpp"

namespace
{
Trigger::Jabber::AccountConfig getTestConfig1(void)
{
  return Trigger::Jabber::AccountConfig(JABBER1_TEST_ACCOUNT_SERVER,
                                        JABBER1_TEST_ACCOUNT_LOGIN,
                                        JABBER1_TEST_ACCOUNT_PASS);
}

Trigger::Jabber::AccountConfig getTestConfig2(void)
{
  return Trigger::Jabber::AccountConfig(JABBER2_TEST_ACCOUNT_SERVER,
                                        JABBER2_TEST_ACCOUNT_LOGIN,
                                        JABBER2_TEST_ACCOUNT_PASS);
}

// helper for backward compatibility
Trigger::Jabber::AccountConfig getTestConfig(void)
{
  return getTestConfig1();
}
struct MessageHandler
{
  std::string msg_;
  std::string sender_;
};

static LmHandlerResult
handleMessages (LmMessageHandler * /*handler*/,
                LmConnection     * /*connection*/,
                LmMessage        *m,
                MessageHandler   *mh)
{
  if ((lm_message_get_sub_type (m) != LM_MESSAGE_SUB_TYPE_CHAT &&
       lm_message_get_sub_type (m) != LM_MESSAGE_SUB_TYPE_NORMAL)) {

    return LM_HANDLER_RESULT_ALLOW_MORE_HANDLERS;
  }
  std::string sender(lm_message_node_get_attribute (m->node, "from"));
  mh->sender_ = sender;
  std::string msg(lm_message_node_get_value( lm_message_node_get_child(m->node, "body") ));
  mh->msg_ = msg;
  return LM_HANDLER_RESULT_REMOVE_MESSAGE;
}

std::string getMessageFromAccount(const Trigger::Jabber::AccountConfig &account, const std::string sender)
{
  Trigger::Jabber::Connection conn(account);
  MessageHandler mh;
  LmMessage *m = lm_message_new(NULL, LM_MESSAGE_TYPE_PRESENCE);
  LmMessageHandler *handler = lm_message_handler_new ((LmHandleMessageFunction)handleMessages, &mh, NULL);
  lm_connection_register_message_handler(conn.get(), handler,
                                         LM_MESSAGE_TYPE_MESSAGE,
                                         LM_HANDLER_PRIORITY_NORMAL);

  lm_message_handler_unref (handler);
  lm_connection_send(conn.get(), m, NULL);
  lm_message_unref(m);
  for(;;)
  {
    g_main_context_iteration(NULL, FALSE);
    replace_last(mh.sender_, "/acarm-ng", "");
    if(mh.sender_ != sender)
      continue;
    return mh.msg_;
  }
}

} // unnamed namespace

#endif
