/*
 * UniqueTypeString.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <stdexcept>

#include "System/UniqueTypeString.hpp"
#include "System/ignore.hpp"

using namespace System;

namespace
{
bool g_blockCopy;
struct BlockCopyType
{
  BlockCopyType(void)
  {
    if(g_blockCopy)
      throw std::runtime_error("generic std::swap() has been called (1)");
  }
  BlockCopyType(const BlockCopyType &)
  {
    if(g_blockCopy)
      throw std::runtime_error("generic std::swap() has been called (2)");
  }
};

namespace detail
{
struct BlockCopy
{
  typedef BlockCopyType StringType;
};
} // unnamed namespace detail

typedef UniqueTypeString<detail::BlockCopy> BlockCopy;
} // unnamed namespace

namespace std
{
template<>
void swap(BlockCopyType &, BlockCopyType &)
{
  // ignore this call
} // swap()
} // namespace std


namespace
{

namespace detail
{
struct StrType
{
  typedef std::string StringType;
};
} // unnamed namespace
typedef UniqueTypeString<detail::StrType> StrType;

namespace detail
{
struct ConstCharType
{
  typedef const char * StringType;
};
} // unnamed namespace
typedef UniqueTypeString<detail::ConstCharType> ConstCharType;

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;
factory tf("System/UniqueTypeString");
}


namespace tut
{

// test c-tor
template<>
template<>
void testObj::test<1>(void)
{
  const StrType t("abc");
  ensure_equals("invalid string saved", t.str(), "abc");
}

// test copy c-tor
template<>
template<>
void testObj::test<2>(void)
{
  const StrType a("aaa");
  const StrType b(a);
  ensure_equals("invalid copy", b.str(), a.str() );
}

// test assignment
template<>
template<>
void testObj::test<3>(void)
{
  const StrType a("aaa");
  StrType       b("bbb");
  b=a;
  ensure_equals("invalid copy", b.str(), a.str() );
}

// test equality
template<>
template<>
void testObj::test<4>(void)
{
  const StrType a("aaa");
  const StrType b(a);
  ensure("identical objects not equal", a==b);
}

// test < operator
template<>
template<>
void testObj::test<5>(void)
{
  const StrType a("a");
  const StrType b("b");
  ensure("invalid order 1",   a<b );
  ensure("invalid order 2", !(b<a));
}

// test > operator
template<>
template<>
void testObj::test<6>(void)
{
  const StrType a("a");
  const StrType b("b");
  ensure("invalid order 1",   b>a );
  ensure("invalid order 2", !(a>b));
}

// test != operator
template<>
template<>
void testObj::test<7>(void)
{
  const StrType a("a");
  const StrType b("b");
  ensure("objects reported identical",   a!=b );
  ensure("object not identical",       !(a!=a));
}

// test < operator for const char* specialization
template<>
template<>
void testObj::test<8>(void)
{
  char str[]="b\0a";
  const ConstCharType a(str+2);
  const ConstCharType b(str+0);
  ensure("invalid order in memory", a.str()>b.str() );
  ensure("invalid order 1",   a<b );
  ensure("invalid order 2", !(b<a));
}

// test value_type typedef
template<>
template<>
void testObj::test<9>(void)
{
  const ConstCharType       a("abc");
  ConstCharType::value_type v=a.str();
  ignore(v);
}

// test swap method
template<>
template<>
void testObj::test<10>(void)
{
  StrType a("abc");
  StrType b("def");
  a.swap(b);
  ensure_equals("invalid string 'a'", a.str(), "def");
  ensure_equals("invalid string 'b'", b.str(), "abc");
}

// test swap function
template<>
template<>
void testObj::test<11>(void)
{
  StrType a("abc");
  StrType b("def");
  std::swap(a, b);
  ensure_equals("invalid string 'a'", a.str(), "def");
  ensure_equals("invalid string 'b'", b.str(), "abc");
}

// test if swap function is called, instead of standard implementaiton
template<>
template<>
void testObj::test<12>(void)
{
  g_blockCopy=false;
  BlockCopy a( (BlockCopyType()) );
  BlockCopy b( (BlockCopyType()) );
  g_blockCopy=true;
  std::swap(a, b);
}

} // namespace tut
