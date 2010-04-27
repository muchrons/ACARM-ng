/*
 * Path.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Path.hpp"

using namespace Preprocessor;

namespace
{

struct TestClass
{
  size_t count(const Path &p) const
  {
    size_t c=0;
    for(Path::const_iterator it=p.begin(); it!=p.end(); ++it)
      ++c;
    return c;
  }

  void testThrow(const char *path) const
  {
    try
    {
      Path p(path);     // should throw
      tut::fail("c-tor didn't throw on invalid path");
    }
    catch(const ExceptionInvalidPath &)
    {
      // this is expected
    }
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/Path");
} // unnamed namespace


namespace tut
{

// test tokenizing minimal path
template<>
template<>
void testObj::test<1>(void)
{
  const Path           path("abc.*");
  Path::const_iterator it=path.begin();
  ensure_equals("invalid count", count(path), 2u);

  // 1
  ensure("not enought elements in collection /1", it!=path.end() );
  ensure_equals("invalid first token", *it, "abc");
  ++it;
  // 2
  ensure("not enought elements in collection /2", it!=path.end() );
  ensure_equals("invalid second token", *it, "*");
  ++it;
  // end
  ensure("too many tokens", it==path.end() );
}

// test tokeninzing maximal path
template<>
template<>
void testObj::test<2>(void)
{
  const Path           path("1.2.3.4.5.6");
  Path::const_iterator it=path.begin();
  ensure_equals("invalid count", count(path), 6u);
}

// test too long path
template<>
template<>
void testObj::test<3>(void)
{
  testThrow("short");
}

// test too long path
template<>
template<>
void testObj::test<4>(void)
{
  testThrow("1.2.3.4.5.6.toolong");
}

// test getting path from object
template<>
template<>
void testObj::test<5>(void)
{
  const Path p("1.2.3");
  ensure_equals("invalid path", p.get(), "1.2.3");
}

} // namespace tut
