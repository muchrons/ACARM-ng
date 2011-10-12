/*
 * TestCallback.t.hpp
 *
 */
#ifndef INCLUDE_DATAFACADES_SACCESS_TESTCALLBACK_T_HPP_FILE
#define INCLUDE_DATAFACADES_SACCESS_TESTCALLBACK_T_HPP_FILE

#include <string>

namespace DataFacades
{
namespace StrAccess
{

struct TestCallback
{
  TestCallback(void):
    lastValue_("NOTHING's SET YET"),
    lastSize_(69696969),
    lastNullFound_("NO NULLs FOUND YET")
  {
  }

  bool collectionSize(size_t size)
  {
    lastSize_=size;
    return true;
  }

  bool value(const std::string &v)
  {
    lastValue_=v;
    return true;
  }

  bool nullOnPath(const std::string &where)
  {
    lastNullFound_=where;
    return true;
  }

  std::string lastValue_;
  size_t      lastSize_;
  std::string lastNullFound_;
}; // strict TestCallback

} // namespace StrAccess
} // namespace DataFacades

#endif
