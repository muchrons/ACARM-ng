/*
 * TestCallback.t.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_FACADES_SACCESS_TESTCALLBACK_T_HPP_FILE
#define INCLUDE_PERSISTENCY_FACADES_SACCESS_TESTCALLBACK_T_HPP_FILE

#include <string>
#include <inttypes.h>

namespace
{

struct TestCallback
{
  TestCallback(void):
    lastValue_("NOTHING's SET YET"),
    lastSize_(69696969)
  {
  }

  virtual void collectionSize(size_t size)
  {
    lastSize_=size;
  }

  virtual void value(const std::string &v)
  {
    lastValue_=v;
  }

  std::string lastValue_;
  size_t      lastSize_;
}; // strict TestCallback

} // unnamed namespace

#endif
