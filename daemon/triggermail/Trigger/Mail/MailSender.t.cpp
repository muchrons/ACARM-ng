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

// test if sending with invalid certificate fails
template<>
template<>
void testObj::test<2>(void)
{
  const Config::Authorization auth(MAIL1_TEST_ACCOUNT_LOGIN,
                                   MAIL1_TEST_ACCOUNT_PASS);
  const Config::Server        srv(MAIL1_TEST_ACCOUNT_SERVER,
                                  MAIL1_TEST_ACCOUNT_PORT,
                                  Trigger::Mail::Config::Server::Protocol::MAIL1_TEST_ACCOUNT_PROTOCOL,
                                  Trigger::Mail::Config::Server::Security::MAIL1_TEST_ACCOUNT_SECURITY,
                                  "testdata/invalid_cert.pem");
  const Trigger::Simple::ThresholdConfig th("0", "0");
  const Config cfg(th, MAIL1_TEST_ACCOUNT_ADDRESS, Config::Recipients(MAIL2_TEST_ACCOUNT_ADDRESS), srv, auth);

  // send report
  MailSender ms(cfg);
  try
  {
    ms.send("subject", "content");  // should throw
    fail("invalid certificate didn't raised an error");
  }
  catch(const ExceptionInvalidCertificate &)
  {
    // this is expected
  }
}

// TODO: test sending messages via SMTP/TLS

// TODO: test sending messages via SMTP/SSL

} // namespace tut
