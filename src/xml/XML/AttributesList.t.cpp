/*
 * AttributesList.t.cpp
 *
 */
#include <tut.h>

#include "XML/AttributesList.hpp"


namespace XML
{

struct AttributesListTestClass
{
  typedef AttributesList AL;

  AttributesListTestClass(void):
    a1_("n1", "v1"),
    a2_("n2", "v2")
  {
  }

  const XML::Attribute a1_;
  const XML::Attribute a2_;
  AL                   al_;
};

} // namespace XML


namespace tut
{
typedef XML::AttributesListTestClass TestClass;
typedef test_group<TestClass> factory;
typedef factory::object testObj;
} // namespace tut


namespace
{
tut::factory tf("XML/AttributesList");
}

using namespace std;
using namespace XML;


namespace tut
{

// check if construcotr creates empty collection.
template<>
template<>
void testObj::test<1>(void)
{
  ensure("initial collection is not empty", al_.begin()==al_.end() );
}

// try adding some element.
template<>
template<>
void testObj::test<2>(void)
{
  al_.add(a1_); // should be ok.
  ensure("addition failed but didn't throw", al_.begin()!=al_.end() );
  // check if attribute matches set one.
  const Attribute &attr=*al_.begin();
  ensure("invalid attribute has been returned",
         attr.getName() ==a1_.getName() &&
         attr.getValue()==a1_.getValue() );
  // check if this is the last element.
  AL::const_iterator it=al_.begin();
  ++it;
  ensure("more that one element in collection", it==al_.end() );
}

// try if collection will add multiple elements
template<>
template<>
void testObj::test<3>(void)
{
  // following code should not throw
  al_.add(a1_);
  al_.add(a2_);
}

// test if collection will throw on addin element
// with the same name as already added.
template<>
template<>
void testObj::test<4>(void)
{
  al_.add(a1_);
  al_.add(a2_);
  const Attribute attr(a2_.getName(), "some value");
  try
  {
    al_.add(attr);  // should throw
    fail("add() did not throw on duplicated name");
  }
  catch(const XML::Exception&)
  {
    // this is expected
  }
}

// try looking for element by its name
template<>
template<>
void testObj::test<5>(void)
{
  al_.add(a1_);
  al_.add(a2_);
  ensure("element 1 not found", al_.find( a1_.getName() )!=al_.end() );
  ensure("element 2 not found", al_.find( a2_.getName() )!=al_.end() );
  ensure("element 2 not found", al_.find( a2_.getName() )!=al_.end() );
}

// try looking for non-exisitng element
template<>
template<>
void testObj::test<6>(void)
{
  ensure("empty list retunred some element", al_.find("xyz")==al_.end() );
  al_.add(a1_);
  al_.add(a2_);
  ensure("non-existing element found 1", al_.find("cat"  )==al_.end() );
  ensure("non-existing element found 2", al_.find("alice")==al_.end() );
}

// get some element by the name
template<>
template<>
void testObj::test<7>(void)
{
  al_.add(a1_);
  al_.add(a2_);
  const Attribute &a=al_.getAttribute("n1");
  ensure_equals("invalid name of returned attribute",
                a.getName(), "n1");
  ensure_equals("invalid value of returned attribute",
                a.getValue(), "v1");
}

// test throw on get() of unexisitng attribute
template<>
template<>
void testObj::test<8>(void)
{
  al_.add(a1_);
  al_.add(a2_);
  try
  {
    al_.getAttribute("non_exisitng_element");
    fail("getAttribute() didn't throw on non-existing attribute");
  }
  catch(const Exception&)
  {
    // this is expected
  }
}

} // namespace tut
