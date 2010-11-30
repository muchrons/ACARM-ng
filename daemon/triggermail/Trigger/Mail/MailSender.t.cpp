/*
 * MailSender.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/Mail/MailSender.hpp"
#include "Trigger/Mail/TestAccount.t.hpp"

using namespace Trigger::Mail;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Mail/MailSender");
} // unnamed namespace


namespace tut
{

// test by sending message
template<>
template<>
void testObj::test<1>(void)
{
  // wipe-out account's content
  removeMessagesFromAccount( getTestConfig2() );
  // send report
  MailSender ms( getTestConfig1() );
  ms.send("subject 1", "some\ndata parts\nwith\n\n\nnew lines");
  // check results
  const int count=removeMessagesFromAccount( getTestConfig2(), 1 );
  ensure_equals("invalid number of messages removed", count, 1);
}

// TODO: test sending messages via SMTP/TLS

// TODO: test sending messages via SMTP/SSL

} // namespace tut
