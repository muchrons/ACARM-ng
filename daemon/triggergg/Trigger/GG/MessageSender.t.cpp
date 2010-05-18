/*
 * MessageSender.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/GG/MessageSender.hpp"
#include "Trigger/GG/TestAccount.t.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Trigger::GG;
using namespace TestHelpers::Persistency;

namespace
{

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


struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    conn1_( getTestConfig1() ),
    conn2_( getTestConfig2() )
  {
  }

  std::string getMessageFromAccount2(void)
  {
    for(;;)
    {
      EventWrapper e( gg_watch_fd( conn2_.get() ) );    // read event
      // skip errors
      if( e.get()==NULL )
        continue;
      // we wait for message
      if( e.get()->type!=GG_EVENT_MSG )
        continue;
      // get reference to message
      gg_event_msg &m=e.get()->event.msg;
      // only messages from given UID are interesting for us
      if( m.sender!=getTestConfig1().getUserID() )
        continue;

      // ok - this is our message!
      tut::ensure("NULL message received", m.message!=NULL);
      const char *msg=reinterpret_cast<const char*>(m.message);
      return msg;
    } // for(events)
  }

  Connection conn1_;
  Connection conn2_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/GG/MessageSender");
} // unnamed namespace


namespace tut
{

// test sending message from one account to another.
template<>
template<>
void testObj::test<1>(void)
{
  MessageSender     ms(conn1_);
  const std::string msg("alice has a cat");
  ms.send( getTestConfig2().getUserID(), msg );
  const std::string recv=getMessageFromAccount2();
  ensure_equals("invalid message received", recv, msg);
}

// test c-tor/d-tor
template<>
template<>
void testObj::test<2>(void)
{
  MessageSender ms(conn1_);
}

// test sending UTF8 message
template<>
template<>
void testObj::test<3>(void)
{
  MessageSender     ms(conn1_);
  const std::string msg("łączność UTF-8");
  ms.send( getTestConfig2().getUserID(), msg );
  const std::string recv=getMessageFromAccount2();
  ensure_equals("invalid message received", recv, msg);
}

} // namespace tut
