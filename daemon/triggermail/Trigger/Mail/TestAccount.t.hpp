/*
 * TestAccount.t.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_MAIL_TESTACCOUNT_T_HPP_FILE
#define INCLUDE_TRIGGER_MAIL_TESTACCOUNT_T_HPP_FILE

#include <string>
#include <boost/noncopyable.hpp>
#include <unistd.h>
#include <libetpan/libetpan.h>
#include <cassert>

#include "System/ScopedPtrCustom.hpp"
#include "Trigger/Mail/Config.hpp"
#include "Trigger/Mail/LoggerWrapper.hpp"
#include "TestHelpers/Data/mail1.hpp"
#include "TestHelpers/Data/mail2.hpp"

namespace
{

Trigger::Mail::Config getTestConfig1(const char *to=MAIL2_TEST_ACCOUNT_ADDRESS)
{
  const Trigger::Mail::Config::Authorization auth(MAIL1_TEST_ACCOUNT_LOGIN,
                                                  MAIL1_TEST_ACCOUNT_PASS);
  const Trigger::Mail::Config::Server        srv(MAIL1_TEST_ACCOUNT_ADDRESS,
                                                 MAIL1_TEST_ACCOUNT_SERVER,
                                                 MAIL1_TEST_ACCOUNT_PORT,
                                                 Trigger::Mail::Config::Server::Security::SSL);
  const Trigger::Simple::ThresholdConfig     th("0", "0");
  return Trigger::Mail::Config(th, to, srv, auth);
}

Trigger::Mail::Config getTestConfig2(void)
{
  const Trigger::Mail::Config::Authorization auth(MAIL2_TEST_ACCOUNT_LOGIN,
                                                  MAIL2_TEST_ACCOUNT_PASS);
  const Trigger::Mail::Config::Server        srv(MAIL2_TEST_ACCOUNT_ADDRESS,
                                                 MAIL2_TEST_ACCOUNT_SERVER,
                                                 MAIL2_TEST_ACCOUNT_PORT,
                                                 Trigger::Mail::Config::Server::Security::SSL);
  const Trigger::Simple::ThresholdConfig     th("0", "0");
  return Trigger::Mail::Config(th, MAIL1_TEST_ACCOUNT_ADDRESS, srv, auth);
}


typedef System::ScopedPtrCustom<mailstorage, mailstorage_free> StorageHolderHelper;
typedef System::ScopedPtrCustom<mailfolder,  mailfolder_free>  FolderHolderHelper;

// internal (helper) implementation
int removeMessagesFromAccountImpl(const Trigger::Mail::Config &cfg)
{
  StorageHolderHelper storage( mailstorage_new(NULL) );             // create storage
  if(storage.get()==NULL)
    throw std::runtime_error("storage() allocation error");
  assert( cfg.getAuthorizationConfig()!=NULL );
  if( pop3_mailstorage_init(storage.get(),
                            MAIL2_TEST_ACCOUNT_POP_SERVER,
                            MAIL2_TEST_ACCOUNT_POP_PORT,
                            NULL,
                            CONNECTION_TYPE_TLS,
                            POP3_AUTH_TYPE_PLAIN,
                            cfg.getAuthorizationConfig()->user_.c_str(),
                            cfg.getAuthorizationConfig()->pass_.c_str(),
                            0, NULL,
                            NULL) != MAIL_NO_ERROR )                // init POP3 storage
    throw std::runtime_error("pop3_mailstorage_init() unable to init POP3 storage");
  FolderHolderHelper folder( mailfolder_new(storage.get(), "/a/b/c/d", NULL) ); // create folder
  if( mailfolder_connect( folder.get() ) != MAIL_NO_ERROR )         // connect to folder
    throw std::runtime_error("mailfolder_connect() unable to connect to folder");

  // loop to read (and delete messages)
  int count=0;
  for(uint32_t index=1; /* break inside the loop */; ++index)
  {
    {
      mailmessage *msg;
      if( mailsession_get_message(folder->fld_session, index, &msg) != MAIL_NO_ERROR )
        break;                  // no more messages
      mailmessage_free(msg);    // we don't actually need this message :)
    }

    ++count;                    // ok - one more message

    // remove message from server
    if( mailsession_remove_message(folder->fld_session, index) != MAIL_NO_ERROR )
      throw std::runtime_error("mailsession_remove_message(): unable to remove message");
  } // for(all_messages)

  return count;
} // getMessageFromAccount()


// returns number of removed messages.
int removeMessagesFromAccount(const Trigger::Mail::Config &cfg, int minCount=0)
{
  Trigger::Mail::LoggerWrapper logWrp;                  // add logs to log file
  const time_t                 deadline=time(NULL)+45;  // give it 45[s] timeout
  int                          count   =0;              // no elements removed yet
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
