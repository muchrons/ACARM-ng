/*
 * Config.t.cpp
 *
 */
#include <tut.h>

#include "ConfigIO/Preprocessor/Config.hpp"

using namespace std;
using namespace ConfigIO::Preprocessor;

namespace
{
struct TestClass
{
  Config::Sections makeSections(size_t size=0) const
  {
    Config::Sections s;
    for(size_t i=0; i<size; ++i)
      s.push_back( Section(Section::Type::ACCEPT, Expression::makeTerm( Rule::makeTrue() ) ) );
    return s;
  }

};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/Preprocessor/Config");
} // unnamed namespace


namespace tut
{

// test making empty sections.
template<>
template<>
void testObj::test<1>(void)
{
  const Config c( makeSections() );
  // TODO: misspell 'elements'
  ensure_equals("invalid elemsnts count", c.getSections().size(), 0u);
}

// test non-empty section
template<>
template<>
void testObj::test<2>(void)
{
  const Config c( makeSections(42) );
  // TODO: misspell 'elements'
  ensure_equals("invalid elemsnts count", c.getSections().size(), 42u);
}

} // namespace tut
