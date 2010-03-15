/*
 * DShieldParser.t.cpp
 *
 */
#include <tut.h>

#include "Filter/IPBlackList/DShieldParser.hpp"

using namespace std;
using namespace Filter::IPBlackList;

namespace
{

struct TestClass
{
  IP ip(const char *str) const
  {
    assert(str!=NULL);
    return IP::from_string(str);
  }

  void checkParsing(const char *in, const char *ip1, const char *ip2) const
  {
    DShieldParser                 p(in);
    DShieldParser::const_iterator it=p.begin();

    if(ip1==NULL)
    {
      // end?
      tut::ensure("no first element", it==p.end() );
      return;
    }
    // first entry
    tut::ensure("empty collection", it!=p.end() );
    tut::ensure_equals("invalid first element", *it, ip(ip1) );
    ++it;

    if(ip2==NULL)
    {
      // end?
      tut::ensure("no second element", it==p.end() );
      return;
    }
    // second entry
    tut::ensure("no second entry", it!=p.end() );
    tut::ensure_equals("invalid second element", *it, ip(ip2) );
    ++it;

    // end?
    tut::ensure("too many elements in collection", it==p.end() );
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/IPBlackList/DShieldParser");
} // unnamed namespace


namespace tut
{

// test parsing sample data
template<>
template<>
void testObj::test<1>(void)
{
  const char *in=
    "# some comment\n"
    "218.244.231.132 173049  96320   2009-09-28      2010-03-08\n"
    "218.008.251.187 245502  90990   2009-10-02      2010-03-07\n"
    "# some comment"
    ;
  checkParsing(in, "218.244.231.132", "218.8.251.187");
}

// test comparing with end iterator
template<>
template<>
void testObj::test<2>(void)
{
  const DShieldParser p("");
  ensure("end iterator does not point to end", p.begin()==p.end() );
}

// test empty input
template<>
template<>
void testObj::test<3>(void)
{
  checkParsing("", NULL, NULL);
}

// test comments-only input
template<>
template<>
void testObj::test<4>(void)
{
  const char *in=
    "# some comment\n"
    "# some comment"
    ;
  checkParsing(in, NULL, NULL);
}

// test no-comments input
template<>
template<>
void testObj::test<5>(void)
{
  const char *in=
    "201.202.203.204 173049  96320   2009-09-28      2010-03-08\n"
    "218.008.251.187 245502  90990   2009-10-02      2010-03-07\n"
    ;
  checkParsing(in, "201.202.203.204", "218.8.251.187");
}

// test too short line
template<>
template<>
void testObj::test<6>(void)
{
  const char *in=
    "# some comment\n"
    "218.244.231.13\n"
    "# some comment"
    ;
  DShieldParser                 p(in);
  DShieldParser::const_iterator it=p.begin();
  try
  {
    *it;
    fail("too-short IP has been dereferenced successfully");
  }
  catch(const ExceptionInvalidFormat &)
  {
    // this is expected
  }
}

// test empty line(s)
template<>
template<>
void testObj::test<7>(void)
{
  const char *in=
    "# some comment\n"
    "\n"
    "218.244.231.132 173049  96320   2009-09-28      2010-03-08\n"
    "\n"
    "\n"
    "218.008.251.187 245502  90990   2009-10-02      2010-03-07\n"
    "\n"
    "# some comment"
    ;
  checkParsing(in, "218.244.231.132", "218.8.251.187");
}

// test different DOS-style line ending
template<>
template<>
void testObj::test<8>(void)
{
  const char *in=
    "# some comment\r\n"
    "218.244.231.132 173049  96320   2009-09-28      2010-03-08\r\n"
    "218.008.251.187 245502  90990   2009-10-02      2010-03-07\r\n"
    "# some comment\r\n"
    ;
  checkParsing(in, "218.244.231.132", "218.8.251.187");
}

// test different MAC-style line ending
template<>
template<>
void testObj::test<9>(void)
{
  const char *in=
    "# some comment\r"
    "218.244.231.132 173049  96320   2009-09-28      2010-03-08\r"
    "218.008.251.187 245502  90990   2009-10-02      2010-03-07\r"
    "# some comment\r"
    ;
  checkParsing(in, "218.244.231.132", "218.8.251.187");
}

// test zero-padded digits
template<>
template<>
void testObj::test<10>(void)
{
  const char *in=
    "001.002.003.004 173049  96320   2009-09-28      2010-03-08\n"
    ;
  checkParsing(in, "1.2.3.4", NULL);
}

// test double dereference on element.
template<>
template<>
void testObj::test<11>(void)
{
  const char *in=
    "# some comment\n"
    "218.244.231.132 173049  96320   2009-09-28      2010-03-08\n"
    "# some comment\n"
    ;
  DShieldParser                 p(in);
  DShieldParser::const_iterator it=p.begin();

  // first entry
  tut::ensure("empty collection", it!=p.end() );
  tut::ensure_equals("invalid first derefernce",  *it, ip("218.244.231.132") );
  tut::ensure_equals("invalid second derefernce", *it, ip("218.244.231.132") );
  ++it;

  // end?
  tut::ensure("too many elements in collection", it==p.end() );
}

// test skipping multiple comments in line
template<>
template<>
void testObj::test<12>(void)
{
  const char *in=
    "# some comment 1\n"
    "# some comment 2\n"
    "# some comment 3\n"
    "218.244.231.132 173049  96320   2009-09-28      2010-03-08\n"
    "218.008.251.187 245502  90990   2009-10-02      2010-03-07\n"
    "# some comment 4\n"
    "# some comment 5\n"
    ;
  checkParsing(in, "218.244.231.132", "218.8.251.187");
}

} // namespace tut
