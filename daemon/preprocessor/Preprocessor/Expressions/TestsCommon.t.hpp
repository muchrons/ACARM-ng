/*
 * TestsCommon.t.cpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_EXPRESSIONS_TESTCOMMON_T_HPP_FILE
#define INCLUDE_PREPROCESSOR_EXPRESSIONS_TESTCOMMON_T_HPP_FILE

#include "Preprocessor/Expressions/Base.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

namespace
{

struct TestExpression: public Preprocessor::Expressions::Base
{
  explicit TestExpression(bool ret):
    ret_(ret),
    calls_(0)
  {
  }

  virtual bool compute(const Persistency::Alert &/*alert*/) const
  {
    ++calls_;
    return ret_;
  }

  bool         ret_;
  mutable int  calls_;
};


Persistency::AlertPtrNN makeAlert(void)
{
  return TestHelpers::Persistency::makeNewAlert();
}

} // unnamed namespace

#endif
