/*
 * TestBase.t.hpp
 *
 */
#ifndef INCLUDE_CONFIGCONSTS_TESTBASE_HPP_FILE
#define INCLUDE_CONFIGCONSTS_TESTBASE_HPP_FILE

#include <tut/tut.hpp>
#include <string>

namespace
{

struct TestBase
{
  void testString(const char *str, const std::string &exp) const
  {
    tut::ensure("string is NULL", str!=NULL);
    tut::ensure_equals("string is valid", str, exp);
  }
}; // struct TestBase

} // unnamed namespace

#endif
