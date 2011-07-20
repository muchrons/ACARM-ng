/*
 * TestCallback.t.hpp
 *
 */
#include "Persistency/Facades/StrAccess/ResultCallback.hpp"

namespace
{

struct TestCallback: public Persistency::Facades::StrAccess::ResultCallback
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
