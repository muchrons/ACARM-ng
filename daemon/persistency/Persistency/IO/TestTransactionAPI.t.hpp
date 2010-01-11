/*
 * TestTransactionAPI.t.cpp
 *
 */
#ifndef INCLUDE_TESTTRANSACTIONAPI_T_HPP_FILE
#define INCLUDE_TESTTRANSACTIONAPI_T_HPP_FILE

#include "Persistency/IO/Transaction.hpp"

namespace
{

struct TestTransactionAPI: public Persistency::IO::Transaction
{
  explicit TestTransactionAPI(int *commits=NULL, int *rollbacks=NULL):
    Persistency::IO::Transaction( getStaticMutex(), "test_transaction"),
    commited_(commits),
    rollbacked_(rollbacks)
  {
  }

  int *commited_;
  int *rollbacked_;

private:
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

  Base::Threads::Mutex &getStaticMutex(void)
  {
    static Base::Threads::Mutex m;
    return m;
  }
}; // struct TestTransactionAPI

} // unnamed namespace

#endif
