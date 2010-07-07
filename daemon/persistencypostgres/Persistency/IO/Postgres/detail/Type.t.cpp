/*
 * Type.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <sstream>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include "Persistency/IO/Postgres/detail/Type.hpp"

#define TYPE_ASSERT(T1, T2) BOOST_STATIC_ASSERT( (boost::is_same<T1, T2>::value) )

using namespace std;
using namespace Persistency::IO::Postgres::detail;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Persistency/IO/Postgres/detail/Type");
} // unnamed namespace


namespace tut
{

// test default template version
template<>
template<>
void testObj::test<1>(void)
{
  const double &tmp=Type<double>::convert(4.2);
  ensure_equals("invalid value", tmp, 4.2);
  TYPE_ASSERT(Type<double>::ReadProxy, double);
}

// test limited NULL string
template<>
template<>
void testObj::test<2>(void)
{
  typedef Commons::LimitedNULLString<42> StrNULL;
  const StrNULL &tmp=Type<StrNULL>::convert("abc");
  ensure("NULL value returned", tmp.get()!=NULL );
  ensure_equals("invalid value", tmp.get(), std::string("abc") );
  TYPE_ASSERT(Type<StrNULL>::ReadProxy, std::string);
}

// test limnited string
template<>
template<>
void testObj::test<3>(void)
{
  typedef Commons::LimitedString<42> Str;
  const Str &tmp=Type<Str>::convert("abc");
  ensure_equals("invalid value", tmp.get(), std::string("abc") );
  TYPE_ASSERT(Type<Str>::ReadProxy, std::string);
}

// test NullValue
template<>
template<>
void testObj::test<4>(void)
{
  typedef Base::NullValue<int> NullInt;
  const NullInt &tmp=Type<NullInt>::convert(42);
  ensure("NULL value returned", tmp.get()!=NULL );
  ensure_equals("invalid value", *tmp.get(), 42);
  TYPE_ASSERT(Type<NullInt>::ReadProxy, int);
}

// test IP address
template<>
template<>
void testObj::test<5>(void)
{
  typedef boost::asio::ip::address IP;
  const IP  orig=boost::asio::ip::address::from_string("1.2.3.4");
  const IP &tmp =Type<IP>::convert("1.2.3.4");
  ensure_equals("invalid value", tmp, orig);
  TYPE_ASSERT(Type<IP>::ReadProxy, std::string);
}

// test timestamp
template<>
template<>
void testObj::test<6>(void)
{
  typedef Persistency::Timestamp TS;
  const char *str="1970.01.01 11:22";
  const TS   &tmp=Type<TS>::convert(str);
  ensure_equals("invalid value", tmp, Persistency::IO::Postgres::timestampFromString(str) );
  TYPE_ASSERT(Type<TS>::ReadProxy, std::string);
}

// test auto_ptr<MD5>
template<>
template<>
void testObj::test<7>(void)
{
  typedef std::auto_ptr<Persistency::MD5Sum> MD5Ptr;
  const char   *md5="12345678901234567890123456789012";
  const MD5Ptr &tmp=Type<MD5Ptr>::convert(md5);
  ensure("NULL value returned", tmp.get()!=NULL );
  ensure_equals("invalid value", tmp->get(), std::string(md5) );
  TYPE_ASSERT(Type<MD5Ptr>::ReadProxy, std::string);
}

// test uint64_t
template<>
template<>
void testObj::test<8>(void)
{
  const string   str="1234";
  const uint64_t val=Type<uint64_t>::convert(str);
  ensure_equals("invalid value", boost::lexical_cast<string>(val), str);
  TYPE_ASSERT(Type<uint64_t>::ReadProxy, std::string);
}

// test uint64_t - negative value
template<>
template<>
void testObj::test<9>(void)
{
  try
  {
    Type<uint64_t>::convert("-123");
    fail("convert() didn't throw on negative value");
  }
  catch(const boost::numeric::negative_overflow &)
  {
    // this is expected
  }
}

} // namespace tut
