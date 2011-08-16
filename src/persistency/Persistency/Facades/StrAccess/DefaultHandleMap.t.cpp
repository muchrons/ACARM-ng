/*
 * DefaultHandleMap.t.cpp
 *
 */
#include <tut.h>
#include <boost/mpl/at.hpp>
#include <boost/type_traits/is_same.hpp>

#include "Persistency/Facades/StrAccess/DefaultHandleMap.hpp"

using namespace boost;
using namespace boost::mpl;
using namespace Persistency;
using namespace Persistency::Facades::StrAccess;
using namespace Persistency::Facades::StrAccess::detail;

namespace
{
struct TestClass
{
  template<typename TQuery, typename TExpected>
  void checkType(const char *errMsg) const
  {
    typedef typename at<DefaultHandleMap, TQuery>::type Has;
    tut::ensure(errMsg, is_same<Has, TExpected>::value);
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/StrAccess/DefaultHandleMap");
} // unnamed namespace


namespace tut
{

// test term
template<>
template<>
void testObj::test<1>(void)
{
  checkType<TermHandle, OnTerm>("term handle is not valid");
}

// test error handle
template<>
template<>
void testObj::test<2>(void)
{
  checkType<ErrorHandle, ErrorThrower>("error handle is not valid");
}

// test non-term
template<>
template<>
void testObj::test<3>(void)
{
  checkType<NonTermHandle, OnNonTerm>("non-term handle is not valid");
}

// test presence of collection index
template<>
template<>
void testObj::test<4>(void)
{
  checkType<CollectionIndexHandle, OnCollectionIndex>("colleciton index handle is not valid");
}

// test presence of reference url handle
template<>
template<>
void testObj::test<5>(void)
{
  checkType<ReferenceURL, OnReferenceURL>("ReferenceURL handle is not valid");
}

// test presence of service handle
template<>
template<>
void testObj::test<6>(void)
{
  checkType<Service, OnService>("Service handle is not valid");
}

// test presence of process handle
template<>
template<>
void testObj::test<7>(void)
{
  checkType<Process, OnProcess>("Process handle is not valid");
}

// test presence of host handle
template<>
template<>
void testObj::test<8>(void)
{
  checkType<Host, OnHost>("Host handle is not valid");
}

// test presence of analyzer handle
template<>
template<>
void testObj::test<9>(void)
{
  checkType<Analyzer, OnAnalyzer>("Analyzer handle is not valid");
}

// test presence of alert handle
template<>
template<>
void testObj::test<10>(void)
{
  checkType<Alert, OnAlert>("Alert handle is not valid");
}

} // namespace tut
