/*
 * LocalAnalyzersCache.t.cpp
 *
 */
#include <tut.h>

#include "DataFacades/detail/LocalAnalyzersCache.hpp"

using namespace Persistency;
using namespace Facades::detail;

namespace
{

struct TestClass
{
  LocalAnalyzersCache lac_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("DataFacades/detail/LocalAnalyzersCache");
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
  ensure_equals("invalid hash", h, "381e9df5c8fb7e31d380e0f71f9bf0754054cbbb");
}

// test hash with NULLs
template<>
template<>
void testObj::test<2>(void)
{
  const LocalAnalyzersCache::Hash h=lac_.makeHash("my name", NULL, NULL, NULL);
  ensure_equals("invalid hash", h, "8e7b4177e4abca786b6324c04c56195c4dea31b2");
}

} // namespace tut
