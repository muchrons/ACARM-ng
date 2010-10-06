/*
 * LocalAnalyzersCache.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/Facades/detail/LocalAnalyzersCache.hpp"

using namespace Persistency;
using namespace Persistency::Facades::detail;

namespace
{

struct TestClass
{
  LocalAnalyzersCache lac_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/detail/LocalAnalyzersCache");
} // unnamed namespace


namespace tut
{

// test computing hash from random analyzer
template<>
template<>
void testObj::test<1>(void)
{
  const Analyzer::IP              ip=Analyzer::IP::from_string("1.2.3.4");
  const LocalAnalyzersCache::Hash h =lac_.makeHash("my name", "1.2.3", "linux rulez", &ip);
  ensure_equals("invalid hash", h, "my name/1.2.3/linux rulez/1.2.3.4");
}

// test hash with NULLs
template<>
template<>
void testObj::test<2>(void)
{
  const LocalAnalyzersCache::Hash h=lac_.makeHash("my name", NULL, NULL, NULL);
  ensure_equals("invalid hash", h, "my name/<NULL>/<NULL>/<NULL>");
}

} // namespace tut
