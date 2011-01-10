/*
 * MetaAlert.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "Persistency/MetaAlert.hpp"
#include "Persistency/TestHelpers.t.hpp"
#include "TestHelpers/checkEquality.hpp"

using namespace std;
using namespace Persistency;

namespace
{

struct TestClass: private TestBase
{
  TestClass(void):
    url_( makeNewReferenceURL() ),
    ma_( "name 1", 42, 4.2, url_.shared_ptr(), ts_, 23u )
  {
  }

  ReferenceURLPtrNN url_;
  const Timestamp   ts_;
  MetaAlert         ma_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/MetaAlert");
} // unnamed namespace


namespace tut
{

// test creting meta-alert from alert
template<>
template<>
void testObj::test<1>(void)
{
  AlertPtrNN  alert=makeNewAlert();
  MetaAlert   ma(alert, 11u);
  ensure_equals("invalid ID", ma.getID().get(), 11u);
  ensure_equals("invalid name", string( ma.getName().get() ),
                                alert->getName().get() );
  ensure_equals("invalid severity delta", ma.getSeverityDelta(), 0);
  ensure_equals("invalid certanity delta", ma.getCertaintyDelta(), 0);
  ensure("invalid reference URL", ma.getReferenceURL()==NULL );
  ensure("invalid creation time",
         ma.getCreateTime()==alert->getCreationTime() );
}

// test getting name
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invalid name", ma_.getName().get(), string("name 1") );
}

// test getting severity delta
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("invalid severity delta", ma_.getSeverityDelta(), 42);
}

// test getting ceratnity delta
template<>
template<>
void testObj::test<4>(void)
{
  ensure_equals("invalid certanity delta", ma_.getCertaintyDelta(), 4.2);
}

// test reference url getting
template<>
template<>
void testObj::test<5>(void)
{
  ensure("invalid reference URL", ma_.getReferenceURL()==url_ );
}

// test creation time
template<>
template<>
void testObj::test<6>(void)
{
  ensure("invalid creation time", ma_.getCreateTime()==ts_ );
}

// test equality with different names
template<>
template<>
void testObj::test<7>(void)
{
  const MetaAlert ma("different", 42, 4.2, url_.shared_ptr(), ts_, 23u);
  TestHelpers::checkEquality(ma, ma_);
}

// test equality with different severity
template<>
template<>
void testObj::test<8>(void)
{
  const MetaAlert ma("name 1", 24, 4.2, url_.shared_ptr(), ts_, 23u);
  TestHelpers::checkEquality(ma, ma_);
}

// test equality with different certanity
template<>
template<>
void testObj::test<9>(void)
{
  const MetaAlert ma("name 1", 42, 2.4, url_.shared_ptr(), ts_, 23u);
  TestHelpers::checkEquality(ma, ma_);
}

// test equality with different url
template<>
template<>
void testObj::test<10>(void)
{
  const MetaAlert ma("name 1", 42, 4.2, ReferenceURLPtr(), ts_, 23u);
  TestHelpers::checkEquality(ma, ma_);
}

// test equality with different timestamp
template<>
template<>
void testObj::test<11>(void)
{
  const Timestamp tmp(0);
  const MetaAlert ma("name 1", 42, 4.2, url_.shared_ptr(), tmp, 23u);
  TestHelpers::checkEquality(ma, ma_);
}

// test equality for different object representing the same values
template<>
template<>
void testObj::test<12>(void)
{
  const Timestamp tmp(0);
  const MetaAlert ma1("name 1", 42, 4.2, makeNewReferenceURL().shared_ptr(), tmp, 23u);
  const MetaAlert ma2("name 1", 42, 4.2, makeNewReferenceURL().shared_ptr(), tmp, 23u);
  TestHelpers::checkEquality(ma1, ma2, ma_);
}

// test equality with different ID
template<>
template<>
void testObj::test<13>(void)
{
  const MetaAlert ma("name 1", 42, 4.2, url_.shared_ptr(), ts_, 12345u);
  TestHelpers::checkEquality(ma, ma_);
}

} // namespace tut
