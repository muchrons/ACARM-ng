/*
 * TestAccount.t.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_TESTACCOUNT_T_HPP_FILE
#define INCLUDE_TRIGGER_GG_TESTACCOUNT_T_HPP_FILE

#include <string>
#include <sys/select.h>
#include <libgadu.h>
#include <cassert>

#include "System/ScopedPtrCustom.hpp"
#include "Trigger/GG/AccountConfig.hpp"
#include "Trigger/GG/Connection.hpp"
#include "TestHelpers/Data/gg1.hpp"
#include "TestHelpers/Data/gg2.hpp"
#include "TestHelpers/Data/gg3.hpp"
#include "TestHelpers/Data/gg4.hpp"

namespace
{

Trigger::GG::AccountConfig getTestConfig1(void)
{
  return Trigger::GG::AccountConfig(GG1_TEST_ACCOUNT_UIN, GG1_TEST_ACCOUNT_PASS);
}

Trigger::GG::AccountConfig getTestConfig2(void)
{
  return Trigger::GG::AccountConfig(GG2_TEST_ACCOUNT_UIN, GG2_TEST_ACCOUNT_PASS);
}

Trigger::GG::AccountConfig getTestConfig3(void)
{
  return Trigger::GG::AccountConfig(GG3_TEST_ACCOUNT_UIN, GG3_TEST_ACCOUNT_PASS);
}

Trigger::GG::AccountConfig getTestConfig4(void)
{
  return Trigger::GG::AccountConfig(GG4_TEST_ACCOUNT_UIN, GG4_TEST_ACCOUNT_PASS);
}

// helper for backward compatibility
Trigger::GG::AccountConfig getTestConfig(void)
{
  return getTestConfig1();
}



// helper class that protects event from possible exceptions
typedef System::ScopedPtrCustom<gg_event, gg_event_free> EventWrapper;

std::string getMessageFromAccount(Trigger::GG::Connection &conn, const Trigger::GG::UserID sender)
{
  timeval timeout={20, 0};                          // timeout is 20[s]
  for(;;)
  {
    // wait for something
    fd_set desc;
    FD_ZERO(&desc);
    FD_SET(conn.get()->fd, &desc);

    // note that we assume running on linux here - other implementations
    // can do random changes to timeout
    const timeval previousTimeout=timeout;
    switch( select(conn.get()->fd+1, &desc, NULL, NULL, &timeout) )
    {
      case 0:   // timeout
        throw std::runtime_error("waiting for messages timeouted");

      case -1:  // error
        throw std::runtime_error("select() returned error");

      default:  // OK
        if( !FD_ISSET(conn.get()->fd, &desc) )
          throw std::runtime_error("descriptor is not set - something's wrong");
        break;
    } // switch( select() )
    assert( previousTimeout.tv_sec>=timeout.tv_sec &&
            "non-linux implementation of select() - move 'timeout' "
            "variable declaration inside the loop (note: this may cause "
            "terribly long timeouts!)" );

    // ok - we have some data to be read

    // read data
    EventWrapper e( gg_watch_fd( conn.get() ) );    // read event
    // skip errors
    if( e.get()==NULL )
      continue;
    // we wait for message
    if( e.get()->type!=GG_EVENT_MSG )
      continue;
    // get reference to message
    gg_event_msg &m=e.get()->event.msg;
    // only messages from given UID are interesting for us
    if(m.sender!=sender)
      continue;

    // ok - this is our message!
    const char *msg=reinterpret_cast<const char*>(m.message);
    return msg;
  } // for(events)
} // getMessageFromAccount()

std::string getMessageFromAccount(const Trigger::GG::AccountConfig &account, const Trigger::GG::UserID sender)
{
  Trigger::GG::Connection conn(account);
  return getMessageFromAccount(conn, sender);
} // getMessageFromAccount()

} // unnamed namespace

#endif
