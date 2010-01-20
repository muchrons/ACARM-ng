/*
 * TestTransactionAPI.t.cpp
 *
 */
#ifndef INCLUDE_TESTTRANSACTIONAPI_T_HPP_FILE
#define INCLUDE_TESTTRANSACTIONAPI_T_HPP_FILE

#include "Persistency/IO/TransactionAPI.hpp"

namespace
{

struct TestTransactionAPI: public Persistency::IO::TransactionAPI
{
  explicit TestTransactionAPI(int *commits=NULL, int *rollbacks=NULL):
    Persistency::IO::TransactionAPI( getStaticMutex(), "test_transaction"),
    commited_(commits),
    rollbacked_(rollbacks)
  {
  }
  TestTransactionAPI(Base::Threads::Mutex &mutex,
                     const std::string    &name,
                     int                  *commits=NULL,
                     int                  *rollbacks=NULL):
    Persistency::IO::TransactionAPI(mutex, name),
    commited_(commits),
    rollbacked_(rollbacks)
  {
  }
  virtual void commitImpl(void)
  {
    if(commited_!=NULL)
      ++*commited_;
  }
  virtual void rollbackImpl(void)
  {
    if(rollbacked_!=NULL)
      ++*rollbacked_;
  }

  int                  *commited_;
  int                  *rollbacked_;

private:
  Base::Threads::Mutex &getStaticMutex(void)
  {
    static Base::Threads::Mutex m;
    return m;
  }
}; // struct TestTransactionAPI

} // unnamed namespace

#endif
