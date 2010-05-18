/*
 * TestAccount.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_TESTACCOUNT_T_HPP_FILE
#define INCLUDE_TRIGGER_GG_TESTACCOUNT_T_HPP_FILE

#include <string>
#include <sys/select.h>
#include <libgadu.h>

#include "Trigger/GG/AccountConfig.hpp"
#include "Trigger/GG/Connection.hpp"

namespace
{

Trigger::GG::AccountConfig getTestConfig1(void)
{
  return Trigger::GG::AccountConfig(23436545, "testowekonto");
}

Trigger::GG::AccountConfig getTestConfig2(void)
{
  return Trigger::GG::AccountConfig(23461426, "testowekonto");
}

// helper for backward compatibility
Trigger::GG::AccountConfig getTestConfig(void)
{
  return getTestConfig1();
}



// helper class that protects event from possible exceptions
struct EventWrapper: private boost::noncopyable
{
  explicit EventWrapper(gg_event *e):
    e_(e)
  {
  }

  ~EventWrapper(void)
  {
    if(e_!=NULL)
      gg_event_free(e_);
  }

  gg_event *get(void)
  {
    return e_;
  }

private:
  gg_event *e_;
}; // struct EventWrapper


std::string getMessageFromAccount(const Trigger::GG::AccountConfig &account, const Trigger::GG::UserID sender)
{
  Trigger::GG::Connection conn(account);
  for(;;)
  {
    // wait for something
    fd_set desc;
    FD_ZERO(&desc);
    FD_SET(conn.get()->fd, &desc);

    timeval timeout={3, 0}; // timeout is 3[s]
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

} // unnamed namespace

#endif
