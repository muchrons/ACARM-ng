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

#include "Trigger/Mail/Config.hpp"
#include "Trigger/Mail/LoggerWrapper.hpp"

namespace
{

Trigger::Mail::Config getTestConfig1(const char *to="acarmng.test.account2@gmail.com")
{
  const Trigger::Mail::Config::Authorization auth("acarmng.test.account1@gmail.com",
                                                  "testowekonto");
  const Trigger::Mail::Config::Server        srv("acarmng.test.account1@gmail.com",
                                                 "smtp.googlemail.com",
                                                 465,
                                                 Trigger::Mail::Config::Server::Security::SSL);
  const Trigger::Simple::ThresholdConfig     th(NULL, NULL);
  return Trigger::Mail::Config(th, to, srv, auth);
}

Trigger::Mail::Config getTestConfig2(void)
{
  const Trigger::Mail::Config::Authorization auth("acarmng.test.account2@gmail.com",
                                                  "testowekonto");
  const Trigger::Mail::Config::Server        srv("acarmng.test.account2@gmail.com",
                                                 "smtp.googlemail.com",
                                                 465,
                                                 Trigger::Mail::Config::Server::Security::SSL);
  const Trigger::Simple::ThresholdConfig     th(NULL, NULL);
  return Trigger::Mail::Config(th, "acarmng.test.account1@gmail.com", srv, auth);
}


struct StorageHolderHelper: private boost::noncopyable
{
  explicit StorageHolderHelper(mailstorage *storage):
    storage_(storage)
  {
  }

  ~StorageHolderHelper(void)
  {
    if(storage_!=NULL)
      mailstorage_free(storage_);
  }

  mailstorage *storage_;
}; // struct StorageHolderHelper

struct FolderHolderHelper: private boost::noncopyable
{
  explicit FolderHolderHelper(mailfolder *folder):
    folder_(folder)
  {
  }

  ~FolderHolderHelper(void)
  {
    if(folder_!=NULL)
    {
      mailfolder_disconnect(folder_);
      mailfolder_free(folder_);
    }
  }

  mailfolder *folder_;
}; // struct FolderHolderHelper


// internal (helper) implementation
int removeMessagesFromAccountImpl(const Trigger::Mail::Config &cfg)
{
  StorageHolderHelper storage( mailstorage_new(NULL) );             // create storage
  if(storage.storage_==NULL)
    throw std::runtime_error("storage() allocation error");
  assert( cfg.getAuthorizationConfig()!=NULL );
  if( pop3_mailstorage_init(storage.storage_,
                            "pop.gmail.com",
                            995,
                            NULL,
                            CONNECTION_TYPE_TLS,
                            POP3_AUTH_TYPE_PLAIN,
                            cfg.getAuthorizationConfig()->user_.c_str(),
                            cfg.getAuthorizationConfig()->pass_.c_str(),
                            0, NULL,
                            NULL) != MAIL_NO_ERROR )                // init POP3 storage
    throw std::runtime_error("pop3_mailstorage_init() unable to init POP3 storage");
  FolderHolderHelper folder( mailfolder_new(storage.storage_, "/a/b/c/d", NULL) );  // create folder
  if( mailfolder_connect(folder.folder_) != MAIL_NO_ERROR )         // connect to folder
    throw std::runtime_error("mailfolder_connect() unable to connect to folder");

  // loop to read (and delete messages)
  int count=0;
  for(uint32_t index=1; /* break inside the loop */; ++index)
  {
    {
      mailmessage *msg;
      if( mailsession_get_message(folder.folder_->fld_session, index, &msg) != MAIL_NO_ERROR )
        break;                  // no more messages
      mailmessage_free(msg);    // we don't actually need this message :)
    }

    ++count;                    // ok - one more message

    // remove message from server
    if( mailsession_remove_message(folder.folder_->fld_session, index) != MAIL_NO_ERROR )
      throw std::runtime_error("mailsession_remove_message(): unable to remove message");
  } // for(all_messages)

  return count;
} // getMessageFromAccount()


// returns number of removed messages.
int removeMessagesFromAccount(const Trigger::Mail::Config &cfg, int minCount=0)
{
  Trigger::Mail::LoggerWrapper logWrp;                  // add logs to log file
  const time_t                 deadline=time(NULL)+10;  // give it 10[s] timeout
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
