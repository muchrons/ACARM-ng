/*
 * ip.t.cpp
 *
 */
#include <tut.h>

#include "Algo/Diff/detail/ip.hpp"

using namespace Algo::Diff::detail;
using Algo::Diff::Similarity;

namespace
{

struct TestClass
{
  boost::asio::ip::address ip(const char *str) const
  {
    assert(str!=NULL && "bug in test");
    return boost::asio::ip::address::from_string(str);
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/Diff/detail/ip");
} // unnamed namespace


namespace tut
{

// test comaprison of the same IPs
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("identical IPs differ", compare( ip("1.2.3.4"), ip("1.2.3.4") ).get(), 1);
}

// test comparison of totally different IPs
template<>
template<>
void testObj::test<2>(void)
{
  ensure("different IPs are too similar", compare( ip("250.250.250.250"), ip("1.1.1.1") ).get() < 0.01);
}

// test comaprison of very simillar IPs
template<>
template<>
void testObj::test<3>(void)
{
  const Similarity s=compare( ip("10.11.12.13"), ip("10.11.12.14") );
  ensure("value is too small", s.get()>0.75);
  ensure("IPs reported identical", s.get()<1);
}

// test comparison of highly different IPs
template<>
template<>
void testObj::test<4>(void)
{
  const Similarity s=compare( ip("10.11.12.13"), ip("10.11.12.14") );
  ensure("value is too small", s.get()>0.75);
  ensure("IPs reported identical", s.get()<1);
}

// test different addresses types
template<>
template<>
void testObj::test<5>(void)
{
  const Similarity s=compare( ip("10.11.12.13"), ip("::abcd") );
  ensure_equals("different IP versions appear not totally different", s.get(), 0);
}

// test different on IPv6 addesses
template<>
template<>
void testObj::test<6>(void)
{
  const Similarity s=compare( ip("::1234:5678:aabb"), ip("::1234:5678:abcd") );
  ensure("value is too small", s.get()>0);
  ensure("IPs reported identical", s.get()<1);
}

// test comaprison of verying addresses
template<>
template<>
void testObj::test<7>(void)
{
  const Similarity s1=compare( ip("10.11.12.13"), ip("10.11.12.12") );
  const Similarity s2=compare( ip("10.11.12.13"), ip("10.11.12.32") );
  ensure("invalid order", s1.get()>s2.get() );
}

// test if difference is only on high bits / IPv6
template<>
template<>
void testObj::test<8>(void)
{
  const Similarity s=compare( ip("1234::1020:3040"), ip("ABCD::1020:3040") );
  ensure("similarity too high", s.get()<0.01);
}

// test if difference is only on high bits / IPv4
template<>
template<>
void testObj::test<9>(void)
{
  const Similarity s=compare( ip("10.11.12.13"), ip("66.11.12.13") );
  ensure("similarity too high", s.get()<0.01);
}

} // namespace tut
