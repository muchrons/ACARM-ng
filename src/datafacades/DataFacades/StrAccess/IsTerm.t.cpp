/*
 * IsTerm.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <vector>

#include "DataFacades/StrAccess/IsTerm.hpp"

using namespace DataFacades::StrAccess;
namespace net=boost::asio::ip;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("DataFacades/StrAccess/IsTerm");
} // unnamed namespace


namespace tut
{

// test if numeric type is term
template<>
template<>
void testObj::test<1>(void)
{
  ensure("int is not term", IsTerm<int>::value);
}

// test other numeric type for being term
template<>
template<>
void testObj::test<2>(void)
{
  ensure("unsigned long is not term", IsTerm<unsigned long>::value);
}

// test for string to be term
template<>
template<>
void testObj::test<3>(void)
{
  ensure("std::string is not term", IsTerm<std::string>::value);
}

// test if const char * is a term
template<>
template<>
void testObj::test<4>(void)
{
  ensure("const char * is not term", IsTerm<const char *>::value);
}

// test if char* is a term
template<>
template<>
void testObj::test<5>(void)
{
  ensure("char * is not term", IsTerm<char *>::value);
}


namespace
{
struct RandomType {};
} // unnamed namespace

// test if sonme random type is not a term
template<>
template<>
void testObj::test<6>(void)
{
  ensure("random type reported to be a term", IsTerm<RandomType>::value==false);
}

// test if collection of terms is not a term
template<>
template<>
void testObj::test<7>(void)
{
  ensure("colleciton of terms reported as a term", IsTerm< std::vector<RandomType> >::value==false);
}

// test if numeric type is term
template<>
template<>
void testObj::test<8>(void)
{
  ensure("int* reported as term", IsTerm<int*>::value==false);
}

namespace
{
template<typename T>
void testCallViaTemplate(const T &/*t*/)
{
  ensure("<const T&> with T=char* found as non-term", IsTerm<T>::value);
} // testCallViaTemplate()
} // unnamed namespace

// test calling IsTerm via template function with const T& arg
template<>
template<>
void testObj::test<9>(void)
{
  testCallViaTemplate("abc");
}

namespace
{
template<typename T>
void testCallViaTemplateNonConst(T &/*t*/)
{
  ensure("<T&> with T=const char* found as non-term", IsTerm<T>::value);
} // testCallViaTemplateNonConst()
} // unnamed namespace

// test calling IsTerm via template function with const T& arg
template<>
template<>
void testObj::test<10>(void)
{
  testCallViaTemplateNonConst("abc");
}

// test if MD5Sum is a term
template<>
template<>
void testObj::test<11>(void)
{
  ensure("MD5Sum is not a term", IsTerm<Persistency::MD5Sum>::value);
}

// test if MD5Sum* is NOT a term
template<>
template<>
void testObj::test<12>(void)
{
  ensure("MD5Sum* is a term", IsTerm<Persistency::MD5Sum*>::value==false);
}

// test if net::address is a term
template<>
template<>
void testObj::test<13>(void)
{
  ensure("address is not a term", IsTerm<net::address>::value);
}

// test if net::address_v4 is a term
template<>
template<>
void testObj::test<14>(void)
{
  ensure("address_v4 is not a term", IsTerm<net::address_v4>::value);
}

// test if net::address_v6 is a term
template<>
template<>
void testObj::test<15>(void)
{
  ensure("address_v6 is not a term", IsTerm<net::address_v6>::value);
}

} // namespace tut
