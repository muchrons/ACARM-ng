/*
 * TestAccount.t.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_JABBER_TESTACCOUNT_T_HPP_FILE
#define INCLUDE_TRIGGER_JABBER_TESTACCOUNT_T_HPP_FILE

#include <glib.h>

#include "System/ScopedPtrCustom.hpp"
#include "Trigger/Jabber/AccountConfig.hpp"
#include "Trigger/Jabber/Connection.hpp"
#include "TestHelpers/Data/jabber1.hpp"
#include "TestHelpers/Data/jabber2.hpp"

namespace
{
static GMainLoop *main_loop = NULL;
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
  static LmHandlerResult
handle_messages (LmMessageHandler *handler,
                 LmConnection     *connection,
                 LmMessage        *m,
                 gpointer          user_data)
{

  g_main_loop_quit (main_loop);
  return LM_HANDLER_RESULT_REMOVE_MESSAGE;
}

std::string getMessageFromAccount(const Trigger::Jabber::AccountConfig &account, const std::string sender)
{
  Trigger::Jabber::Connection conn(account);
  LmMessageHandler *handler = lm_message_handler_new (handle_messages, NULL, NULL);
  lm_connection_register_message_handler (conn.get(), handler,
                                          LM_MESSAGE_TYPE_MESSAGE,
                                          LM_HANDLER_PRIORITY_NORMAL);

  lm_message_handler_unref (handler);
  main_loop = g_main_loop_new (NULL, FALSE);
  g_main_loop_run (main_loop);
  return std::string("");
}

} // unnamed namespace

#endif
