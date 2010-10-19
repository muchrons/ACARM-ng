#include <glib.h>
#include <tut.h>
#include <sys/time.h>
#include <boost/timer.hpp>
#include <boost/algorithm/string.hpp>

#include "Trigger/Jabber/TestAccount.t.hpp"
#include "Trigger/Jabber/Connection.hpp"

namespace Trigger
{

namespace Jabber
{

struct MessageHandler
{
  std::string msg_;
  std::string sender_;
};

AccountConfig getTestConfig1(void)
{
  return Trigger::Jabber::AccountConfig(JABBER1_TEST_ACCOUNT_SERVER,
                                        JABBER1_TEST_ACCOUNT_LOGIN,
                                        JABBER1_TEST_ACCOUNT_PASS);
}

AccountConfig getTestConfig2(void)
{
  return Trigger::Jabber::AccountConfig(JABBER2_TEST_ACCOUNT_SERVER,
                                        JABBER2_TEST_ACCOUNT_LOGIN,
                                        JABBER2_TEST_ACCOUNT_PASS);
}

// helper for backward compatibility
AccountConfig getTestConfig(void)
{
  return getTestConfig1();
}
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
  tut::ensure("message handler is NULL", mh!=NULL);
  mh->sender_ = std::string(lm_message_node_get_attribute (m->node, "from"));
  mh->msg_ = std::string(lm_message_node_get_value( lm_message_node_get_child(m->node, "body") ));
  return LM_HANDLER_RESULT_REMOVE_MESSAGE;
}

std::string getMessageFromAccount(const Trigger::Jabber::AccountConfig &account, const std::string &sender)
{
  Trigger::Jabber::Connection conn(account);
  MessageHandler              mh;
  LmMessage                   *m = lm_message_new(NULL, LM_MESSAGE_TYPE_PRESENCE);
  System::ScopedPtrCustom<LmMessage, lm_message_unref> mPtr(m);
  tut::ensure("message is NULL", mPtr.get()!=NULL);
  // TODO: casting to function pointer to use arg void* with LmHandleMessageFunction is slippy (what if code changes?) - better
  //       use explicit void* as argument to handleMessages() and cast inside the call and be on the safe side. :)
  LmMessageHandler            *handler = lm_message_handler_new((LmHandleMessageFunction)handleMessages, &mh, NULL);
  tut::ensure("message handler is NULL", handler!=NULL);
  lm_connection_register_message_handler(conn.get(), handler,
                                         LM_MESSAGE_TYPE_MESSAGE,
                                         LM_HANDLER_PRIORITY_NORMAL);

  lm_message_handler_unref (handler);
  lm_connection_send(conn.get(), mPtr.get(), NULL);
  const double timeout = 20.0f;      // timeout is 20[s]
  System::Timer t;
  for(;;)
  {
    // short sleep here to avoid waste CPU too much
    usleep(5000);
    // wait for something
    if(t.elapsed() > timeout)
      tut::fail("waiting for messages timeouted");
    g_main_context_iteration(NULL, FALSE);
    // convert sender string to format "login@server"
    boost::replace_last(mh.sender_, "/acarm-ng", "");
    if(mh.sender_ != sender)
      continue;
    return mh.msg_;
  }
}

} // Jabber
} // Trigger