/*
 * TestAccount.t.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_JABBER_TESTACCOUNT_T_HPP_FILE
#define INCLUDE_TRIGGER_JABBER_TESTACCOUNT_T_HPP_FILE

#include <tut.h>
#include <boost/algorithm/string.hpp>
#include <gloox/message.h>
#include <gloox/messagehandler.h>
#include <gloox/messagesessionhandler.h>

#include "System/ScopedPtrCustom.hpp"
#include "System/Timer.hpp"
#include "Trigger/Jabber/AccountConfig.hpp"
#include "Trigger/Jabber/Connection.hpp"
#include "TestHelpers/Data/jabber1.hpp"
#include "TestHelpers/Data/jabber2.hpp"
#include "TestHelpers/Data/jabber3.hpp"

using namespace gloox;
using namespace System;

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

Trigger::Jabber::AccountConfig getTestConfig3(void)
{
  return Trigger::Jabber::AccountConfig(JABBER3_TEST_ACCOUNT_SERVER,
                                        JABBER3_TEST_ACCOUNT_LOGIN,
                                        JABBER3_TEST_ACCOUNT_PASS);
}

// helper for backward compatibility
Trigger::Jabber::AccountConfig getTestConfig(void)
{
  return getTestConfig1();
}


class Handler : public MessageSessionHandler, MessageHandler
{
public:
  Handler(Trigger::Jabber::Connection &conn):
    conn_(conn),
    msg_(false),
    message_(""),
    sess_(0)
  {
    conn_.get()->registerMessageSessionHandler( this );
  }

  virtual void handleMessage(const Message &m, MessageSession *session)
  {
    msg_=true;
    message_=m.body();
    assert(session != NULL);
    sender_ = session->target().username() + "@" + session->target().server();
  }

  bool receiving()
  {
    conn_.get()->recv();
    if(msg_)
      return true;
    return false;
  }

  const std::string &getMessage()
  {
    return message_;
  }

  const std::string &getSender()
  {
    return sender_;
  }

  virtual void handleMessageSession( MessageSession *session )
  {
    if(session)
      conn_.get()->disposeMessageSession( sess_ );
    sess_ = session;
    tut::ensure("session is NULL", sess_!=NULL);
    sess_->registerMessageHandler( this );
  }

private:
  Trigger::Jabber::Connection &conn_;
  bool msg_;
  std::string message_;
  std::string sender_;
  MessageSession *sess_;
};

std::string getMessageFromAccount(const Trigger::Jabber::AccountConfig &account, const std::string &sender)
{
  Trigger::Jabber::Connection conn(account);
  Handler h(conn);
  Timer time;
  const double timeout = 20.0;  // timeout is 20[s]
  while(1)
  {
    // prevent high CPU usage
    usleep(1000);
    // timeout
    if(time.elapsed() > timeout)
      throw std::runtime_error("waiting for messages timeouted");
    // get message
    if(!h.receiving())
      continue;
    // only messages from given sender are intersting for us
    if(h.getSender() != sender)
      continue;
    // ok - this is our message!
    return h.getMessage();
  }
}
} // unnamed namespace

#endif
