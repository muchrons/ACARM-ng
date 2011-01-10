/*
 * Attribute.t.cpp
 *
 */
#include <tut.h>

#include "XML/Attribute.hpp"


namespace XML
{

struct AttributeTestClass
{
};

} // namespace XML


namespace tut
{
typedef XML::AttributeTestClass TestClass;
typedef test_group<TestClass> factory;
typedef factory::object testObj;
} // namespace tut


namespace
{
tut::factory tf("XML/Attribute");
}

using namespace std;
using namespace XML;


namespace tut
{

// check if construciton is ok.
template<>
template<>
void testObj::test<1>(void)
{
  const string    name ("AttributeName" );
  const string    value("AttributeValue");
  const Attribute attr (name, value);
  ensure("name does not match required",  attr.getName() ==name );
  ensure("value does not match required", attr.getValue()==value);
}

} // namespace tut
