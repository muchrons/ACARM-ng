/*
 * EntryControlList.t.cpp
 *
 */
#include <tut.h>

#include "Core/Types/Proc/EntryControlList.hpp"

using namespace Core::Types::Proc;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Core/Types/Proc/EntryControlList");
} // unnamed namespace


namespace tut
{

// test adding new entry to default-accept object - smoke test
template<>
template<>
void testObj::test<1>(void)
{
  EntryControlList ecl=EntryControlList::createDefaultAccept();
  ecl.add("fran");
}

// test adding new entry to default-reject object - somoke test
template<>
template<>
void testObj::test<2>(void)
{
  EntryControlList ecl=EntryControlList::createDefaultReject();
  ecl.add("narf");
}

// test default accepting - positive test
template<>
template<>
void testObj::test<3>(void)
{
  EntryControlList ecl=EntryControlList::createDefaultAccept();
  ecl.add("fran");
  ecl.add("42");
  ensure("entry dropped but not listed", ecl.isAcceptable("DooM") );
}

// test default accepting - negative test
template<>
template<>
void testObj::test<4>(void)
{
  EntryControlList ecl=EntryControlList::createDefaultAccept();
  ecl.add("fran");
  ecl.add("42");
  ensure("entry not dropped but listed", ecl.isAcceptable("42")==false );
}

// test default rejecting - positive test
template<>
template<>
void testObj::test<5>(void)
{
  EntryControlList ecl=EntryControlList::createDefaultReject();
  ecl.add("fran");
  ecl.add("42");
  ensure("entry not dropped but not listed", ecl.isAcceptable("DooM")==false );
}

// test default rejecting - negative test
template<>
template<>
void testObj::test<6>(void)
{
  EntryControlList ecl=EntryControlList::createDefaultReject();
  ecl.add("fran");
  ecl.add("42");
  ensure("entry dropped but not listed", ecl.isAcceptable("42") );
}

// test copyablility
template<>
template<>
void testObj::test<7>(void)
{
  EntryControlList ecl1=EntryControlList::createDefaultReject();
  EntryControlList ecl2=ecl1;       // copy-c-tor
  ecl1=ecl2;                        // assignment
}

} // namespace tut
