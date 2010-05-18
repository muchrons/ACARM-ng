/*
 * TestAccount.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_TESTACCOUNT_T_HPP_FILE
#define INCLUDE_TRIGGER_GG_TESTACCOUNT_T_HPP_FILE

#include <string>
#include <libgadu.h>

#include "Trigger/GG/AccountConfig.hpp"
#include "Trigger/GG/Connection.hpp"

namespace
{

Trigger::GG::AccountConfig getTestConfig1(void)
{
  // Gadu-Gadu server blocks when there are too many connections in short
  // time to the same UID and so we use poll of test accounts that are
  // used in different order, though always in the same (tests must be
  // repeatable!) - random numbers are not welcomed here.
  static size_t index=0;
  const Trigger::GG::UserID uids[]= {
                                      23436545,
 //                                     23461854,
                                      23462737,
                                      23462745,
                                      23461875,
                                      23462759
                                    };
  const size_t pos=( index++ )%( sizeof(uids)/sizeof(uids[0]) );
  return Trigger::GG::AccountConfig(uids[pos], "testowekonto");
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


std::string getMessageFromAccount(const Trigger::GG::AccountConfig &account)
{
  Trigger::GG::Connection conn(account);
  for(;;)
  {
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
    if( m.sender!=account.getUserID() )
      continue;

    // ok - this is our message!
    const char *msg=reinterpret_cast<const char*>(m.message);
    return msg;
  } // for(events)
} // getMessageFromAccount()

} // unnamed namespace

#endif
