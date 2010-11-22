/*
 * TestAccount.t.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_JABBER_TESTACCOUNT_T_HPP_FILE
#define INCLUDE_TRIGGER_JABBER_TESTACCOUNT_T_HPP_FILE

#include <glib.h>
#include <tut.h>
#include <boost/algorithm/string.hpp>
#include <gloox/message.h>
#include <gloox/messagehandler.h>

#include "System/ScopedPtrCustom.hpp"
#include "System/Timer.hpp"
#include "Trigger/Jabber/AccountConfig.hpp"
#include "Trigger/Jabber/Connection.hpp"
#include "TestHelpers/Data/jabber1.hpp"
#include "TestHelpers/Data/jabber2.hpp"

using namespace gloox;
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



class Handler : public MessageHandler
{
  public:
    Handler(Trigger::Jabber::Connection &conn):
      conn_(conn),
      msg_(false),
      message_("")
    {
      conn_.get()->registerMessageHandler( this );
    }
    ~Handler()
    {
    }
    virtual void handleMessage(const Message &m, MessageSession *session)
    {
      msg_=true;
      message_=m.body();
      session->target();
    }
    bool receiving()
    {
        conn_.get()->recv();
        if(msg_)
          return true;
        return false;
    }
    const std::string getMessage()
    {
      return message_;
    }
    const std::string getSender()
    {
      return sender_;
    }
  private:
    Trigger::Jabber::Connection &conn_;
    bool msg_;
    std::string message_;
    std::string sender_;
};

std::string getMessageFromAccount(const Trigger::Jabber::AccountConfig &account, const std::string &sender)
{
  // TODO
  Trigger::Jabber::Connection conn(account);
  Handler h(conn);
  while(1)
  {
    if(!h.receiving())
      continue;
    if(h.getSender() != sender)
      continue;
    return h.getMessage();
  }
}
} // unnamed namespace

#endif
