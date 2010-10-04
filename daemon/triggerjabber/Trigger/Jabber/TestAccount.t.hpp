/*
 * TestAccount.t.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_JABBER_TESTACCOUNT_T_HPP_FILE
#define INCLUDE_TRIGGER_JABBER_TESTACCOUNT_T_HPP_FILE

#include <glib.h>
#include <tut.h>
#include <sys/time.h>
#include <boost/timer.hpp>
#include <boost/algorithm/string.hpp>

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

// TODO: move following code to CPP file

// TODO: this f-ction must be declared as 'extern "C"' in order to be passed to C code.
LmHandlerResult
handleMessages (LmMessageHandler * /*handler*/,
                LmConnection     * /*connection*/,
                LmMessage        *m,
                MessageHandler   *mh)
{
  // TODO: throwing exceptions to C code (here - caller) is undefined behaviour
  tut::ensure("message is NULL", m!=NULL);
  if ((lm_message_get_sub_type (m) != LM_MESSAGE_SUB_TYPE_CHAT &&
       lm_message_get_sub_type (m) != LM_MESSAGE_SUB_TYPE_NORMAL)) {

    return LM_HANDLER_RESULT_ALLOW_MORE_HANDLERS;
  }
  // TODO: segv if mh==NULL
  mh->sender_ = std::string(lm_message_node_get_attribute (m->node, "from"));
  mh->msg_ = std::string(lm_message_node_get_value( lm_message_node_get_child(m->node, "body") ));
  return LM_HANDLER_RESULT_REMOVE_MESSAGE;
}

std::string getMessageFromAccount(const Trigger::Jabber::AccountConfig &account, const std::string &sender)
{
  Trigger::Jabber::Connection conn(account);
  MessageHandler              mh;
  // TODO: use System::ScopedPtrCustom<> for 'm' instead of manual deallocating.
  LmMessage                   *m = lm_message_new(NULL, LM_MESSAGE_TYPE_PRESENCE);
  tut::ensure("message is NULL", m!=NULL);
  // TODO: casting to function pointer to use arg void* with LmHandleMessageFunction is slippy (what if code changes?) - better
  //       use explicit void* as argument to handleMessages() and cast inside the call and be on the safe side. :)
  LmMessageHandler            *handler = lm_message_handler_new((LmHandleMessageFunction)handleMessages, &mh, NULL);
  tut::ensure("message handler is NULL", handler!=NULL);
  lm_connection_register_message_handler(conn.get(), handler,
                                         LM_MESSAGE_TYPE_MESSAGE,
                                         LM_HANDLER_PRIORITY_NORMAL);

  lm_message_handler_unref (handler);
  lm_connection_send(conn.get(), m, NULL);
  lm_message_unref(m);
  const double timeout = 20.0f;      // timeout is 20[s]
  boost::timer t;   // TODO: this clock measures CPU time, instead of real-time, thus on connection isues
                    //       this test may run VERY long. to measure wall time use System::Timer instead.
  for(;;)
  {
    // short sleep here to avoid waste CPU too much
    sleep(1);       // TODO: use usleep() for ~50[ms] here - tests should not last too long
    // wait for something
    if(t.elapsed() > timeout)
      tut::fail("waiting for messages timeouted");
    g_main_context_iteration(NULL, FALSE);
    // TODO: add line of comment/example here - it's non obvoius what is being done here
    boost::replace_last(mh.sender_, "/acarm-ng", "");
    if(mh.sender_ != sender)
      continue;
    return mh.msg_;
  }
}

} // unnamed namespace

#endif
