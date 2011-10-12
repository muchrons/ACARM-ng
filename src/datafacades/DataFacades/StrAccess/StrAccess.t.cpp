/*
 * StrAccess.t.cpp
 *
 */
#include <tut.h>

#include "DataFacades/StrAccess/StrAccess.hpp"
#include "DataFacades/StrAccess/TestBaseData.t.hpp"

using namespace DataFacades::StrAccess;

namespace
{

struct CallbackHandle
{
  bool collectionSize(size_t size)
  {
    last_=Commons::Convert::to<std::string>(size);
    return true;
  }

  bool value(const std::string &v)
  {
    last_=v;
    return true;
  }

  bool nullOnPath(const std::string &where)
  {
    last_="NULL:"+where;
    return true;
  }

  std::string last_;
}; // strict CallbackHandle


typedef Params<DefaultHandleMap, CallbackHandle> MyTestParams;

struct TestClass: public TestBaseData<OnMetaAlert>
{
  CallbackHandle cb_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("DataFacades/StrAccess/StrAccess");
} // unnamed namespace


namespace tut
{

// test processing some random meta-alert
template<>
template<>
void testObj::test<1>(void)
{
  MyTestParams p(Path("metaalert.id"), cb_);
  const bool ret=MainDispatcher::process(metaalertLeaf_, p);
  ensure("invalid exit value", ret);
}

// check callback value
template<>
template<>
void testObj::test<2>(void)
{
  MyTestParams p(Path("metaalert.id"), cb_);
  MainDispatcher::process(metaalertLeaf_, p);
  ensure_equals("invalid value saved", cb_.last_, "665");
}

} // namespace tut
