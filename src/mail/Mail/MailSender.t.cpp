/*
 * MailSender.t.cpp
 *
 */
#include <tut.h>

#include "Mail/MailSender.hpp"
#include "Mail/TestAccount.t.hpp"

using namespace Mail;

namespace
{

struct TestClass
{
  TestClass(void):
    c1_( getTestConfig1() ),
    c2_( getTestConfig2() )
  {
  }

  const Config c1_;
  const Config c2_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Mail/MailSender");
} // unnamed namespace


namespace tut
{

// smoke test for c-tor/d-tor (should not connect)
template<>
template<>
void testObj::test<1>(void)
{
  MailSender ms( getTestConfig1() );
}

// test by sending message
template<>
template<>
void testObj::test<2>(void)
{
  // wipe-out account's content
  removeMessagesFromAccount( getTestConfig2() );
  // send report
  MailSender ms( getTestConfig1() );
  ms.send("subject 1", "some\ndata parts\nwith\n\n\nnew lines", Config:: Recipients(MAIL1_TEST_ACCOUNT_ADDRESS));
  // check results
  const int count=removeMessagesFromAccount( getTestConfig2(), 1 );
  ensure_equals("invalid number of messages removed", count, 1);
}

// test if sending with invalid certificate fails
template<>
template<>
void testObj::test<3>(void)
{
  const Config::Server srv( c1_.getServerConfig().server_,
                                  c1_.getServerConfig().port_,
                                  c1_.getServerConfig().proto_,
                                  c1_.getServerConfig().sec_,
                            "testdata/invalid_cert.pem" );
  ensure("NULL pointer", c1_.getAuthorizationConfig()!=NULL);
  const Config cfg( c1_.getSenderAddress(),
                    srv,
                    *c1_.getAuthorizationConfig() );

  // send report
  MailSender ms(cfg);
  try
  {
    ms.send("subject", "content", Config:: Recipients(MAIL2_TEST_ACCOUNT_ADDRESS));  // should throw
    fail("invalid certificate didn't raised an error");
  }
  catch(const ExceptionInvalidCertificate &)
  {
    // this is expected
  }
}

// try sending mails to multiple recipients
template<>
template<>
void testObj::test<4>(void)
{
  // wipe-out account's content
  removeMessagesFromAccount( getTestConfig1() );
  removeMessagesFromAccount( getTestConfig2() );

  {
    // configure multiple recipients
    Config::Recipients to(MAIL1_TEST_ACCOUNT_ADDRESS);
    to.push_back(MAIL2_TEST_ACCOUNT_ADDRESS);
    ensure("NULL pointer", c1_.getAuthorizationConfig()!=NULL);
    const Config cfg( c1_.getSenderAddress(),
                      c1_.getServerConfig(),
                      *c1_.getAuthorizationConfig() );
    // send report
    MailSender ms(cfg);
    ms.send("subject", "content", to);
  }

  // check first account
  {
    const int count=removeMessagesFromAccount( getTestConfig1(), 1 );
    ensure_equals("invalid number of messages removed / account 1", count, 1);
  }
  // check second account
  {
    const int count=removeMessagesFromAccount( getTestConfig2(), 1 );
    ensure_equals("invalid number of messages removed / account 2", count, 1);
  }
}

// TODO: test sending messages via SMTP/TLS

// TODO: test sending messages via SMTP/SSL

} // namespace tut
