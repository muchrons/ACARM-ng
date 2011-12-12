/*
 * TestBase.t.cpp
 *
 */
#ifndef INCLUDE_PREPROCESSOR_FORMATTERS_TESTBASE_T_HPP_FILE
#define INCLUDE_PREPROCESSOR_FORMATTERS_TESTBASE_T_HPP_FILE

#include "Preprocessor/Formatters/Value.hpp"

using namespace std;
using namespace Preprocessor::Formatters;

namespace
{

template<typename T>
struct TestBase
{
  TestBase(void):
    v_(new Value)
  {
  }

  BasePtrNN make(const size_t args)
  {
    values_.clear();
    Base::Formatters f;
    for(size_t i=0; i<args; ++i)
    {
      ValuePtrNN v(new Value);
      values_.push_back(v);
      f.push_back(v);
    }
    return BasePtrNN(new T(f));
  }

  std::vector<ValuePtrNN> values_;
  ValuePtrNN              v_;
};

} // unnamed namespace

#endif
