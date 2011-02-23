/*
 * TestAccount.t.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_MAIL_TESTACCOUNT_T_HPP_FILE
#define INCLUDE_TRIGGER_MAIL_TESTACCOUNT_T_HPP_FILE

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/lexical_cast.hpp>
#include <unistd.h>
#include <cassert>

#include "System/ScopedPtrCustom.hpp"
#include "Trigger/Mail/Config.hpp"
#include "Trigger/Mail/CertVerifier.hpp"
#include "Trigger/Mail/VmimeHandleInit.hpp"
#include "TestHelpers/Data/mail1.hpp"
#include "TestHelpers/Data/mail2.hpp"

namespace
{

Trigger::Mail::Config getTestConfig1(const char *to=MAIL2_TEST_ACCOUNT_ADDRESS)
{
  const Trigger::Mail::Config::Authorization auth(MAIL1_TEST_ACCOUNT_LOGIN,
                                                  MAIL1_TEST_ACCOUNT_PASS);
  const Trigger::Mail::Config::Server        srv(MAIL1_TEST_ACCOUNT_SERVER,
                                                 MAIL1_TEST_ACCOUNT_PORT,
                                                 Trigger::Mail::Config::Server::Protocol::MAIL1_TEST_ACCOUNT_PROTOCOL,
                                                 Trigger::Mail::Config::Server::Security::MAIL1_TEST_ACCOUNT_SECURITY,
                                                 "testdata/smtp_gmails_root_ca.pem");
  const Trigger::Simple::ThresholdConfig     th("0", "0");
  return Trigger::Mail::Config(th, MAIL1_TEST_ACCOUNT_ADDRESS, Trigger::Mail::Config::Recipients(to), srv, auth);
}

Trigger::Mail::Config getTestConfig2(const char *to=MAIL1_TEST_ACCOUNT_ADDRESS)
{
  const Trigger::Mail::Config::Authorization auth(MAIL2_TEST_ACCOUNT_LOGIN,
                                                  MAIL2_TEST_ACCOUNT_PASS);
  const Trigger::Mail::Config::Server        srv(MAIL2_TEST_ACCOUNT_SERVER,
                                                 MAIL2_TEST_ACCOUNT_PORT,
                                                 Trigger::Mail::Config::Server::Protocol::MAIL2_TEST_ACCOUNT_PROTOCOL,
                                                 Trigger::Mail::Config::Server::Security::MAIL2_TEST_ACCOUNT_SECURITY,
                                                 "testdata/smtp_gmails_root_ca.pem");
  const Trigger::Simple::ThresholdConfig     th("0", "0");
  return Trigger::Mail::Config(th, MAIL2_TEST_ACCOUNT_ADDRESS, Trigger::Mail::Config::Recipients(to), srv, auth);
}


// internal (helper) implementation
int removeMessagesFromAccountImpl(const Trigger::Mail::Config &cfg)
{
  //
  // conect
  //
  Trigger::Mail::VmimeHandleInit  init;
  vmime::utility::url             url("pop3s://" MAIL2_TEST_ACCOUNT_POP_SERVER);
  vmime::ref<vmime::net::session> session  =vmime::create<vmime::net::session>();
  session->getProperties()["store.pop3s.connection.tls"         ]="false";
  session->getProperties()["store.pop3s.connection.tls.required"]="false";
  const Trigger::Mail::Config::Authorization *auth=cfg.getAuthorizationConfig();
  if(auth!=NULL)
  {
    session->getProperties()["store.pop3s.options.need-authentication"]="true";
    session->getProperties()["store.pop3s.auth.username"]=auth->user_;
    session->getProperties()["store.pop3s.auth.password"]=auth->pass_;
  }
  session->getProperties()["store.pop3s.server.address"]=MAIL2_TEST_ACCOUNT_POP_SERVER;
  session->getProperties()["store.pop3s.server.port"   ]=boost::lexical_cast<std::string>(MAIL2_TEST_ACCOUNT_POP_PORT);
  vmime::ref<vmime::net::store> store=session->getStore(url);
  typedef Trigger::Mail::CertVerifier CertVerif;
  const Trigger::Mail::Config::Server srvConfig(MAIL2_TEST_ACCOUNT_POP_SERVER,
                                                MAIL2_TEST_ACCOUNT_POP_PORT,
                                                Trigger::Mail::Config::Server::Protocol::MAIL2_TEST_ACCOUNT_PROTOCOL,
                                                Trigger::Mail::Config::Server::Security::MAIL2_TEST_ACCOUNT_SECURITY,
                                                "testdata/pop_gmails_root_ca.pem");
  vmime::ref<CertVerif>         cv=vmime::ref<CertVerif>::fromPtr( new CertVerif(srvConfig) );
  store->setCertificateVerifier(cv);
  store->connect();

  // get count
  vmime::ref<vmime::net::folder> fld=store->getDefaultFolder();
  fld->open(vmime::net::folder::MODE_READ_WRITE);
  const int count=fld->getMessageCount();

  // clean-up all messages
  if(count>0)
    fld->deleteMessages(1, count);

  //
  // end session
  //
  store->disconnect();
  return count;
} // getMessageFromAccount()


// returns number of removed messages.
int removeMessagesFromAccount(const Trigger::Mail::Config &cfg, int minCount=0)
{
  const time_t deadline=time(NULL)+2*60;                // give it 2[min] timeout
  int          count   =0;                              // no elements removed yet
  // check until timeout's reached, or minimal value is reached
  for(;;)
  {
    count+=removeMessagesFromAccountImpl(cfg);          // test account's content
    if( deadline<time(NULL) || count>=minCount )        // timeout or got expected count?
      break;
    usleep(500*1000);                                   // wait 0.5[s] not to make heavy-busy-loop
                                                        // on fast connections
  }

  return count;
}

} // unnamed namespace

#endif
