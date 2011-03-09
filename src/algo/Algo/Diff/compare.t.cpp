/*
 * compare.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <boost/shared_array.hpp>

#include "System/Math/compareFP.hpp"
#include "Commons/LimitedString.hpp"
#include "Algo/Diff/compare.hpp"

using namespace std;
using namespace Algo::Diff;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/Diff/compare");
} // unnamed namespace


namespace tut
{

// test sample comparison
template<>
template<>
void testObj::test<1>(void)
{
  const string s1("abc");
  const string s2("abc");
  ensure_equals("invalid similarity", compare(s1, s2).get(), 1);
}

// test non-const string comparison
template<>
template<>
void testObj::test<2>(void)
{
  string s1("abc");
  string s2("abc");
  ensure_equals("invalid similarity", compare(s1, s2).get(), 1);
}


namespace
{
typedef Commons::LimitedString<42>            Text;
typedef std::vector<Text>                     Collection;
typedef Commons::SharedPtrNotNULL<Collection> CollectionPtrNN;

CollectionPtrNN makeMultiLevel(const char *text)
{
  Text       txt(text);
  Collection c;
  c.push_back(txt);
  CollectionPtrNN ptr( new Collection(c) );
  return ptr;
} // makeMultiLevel()
} // unnamed namespace

// test multi-lever dispatching to get to the data
template<>
template<>
void testObj::test<3>(void)
{
  const CollectionPtrNN c1 =makeMultiLevel("abc");
  const CollectionPtrNN c2 =makeMultiLevel("abX");
  const double          s  =compare(c1, c2).get();
  const double          exp=2.0/3.0;
  ensure("invalid similarity", System::Math::compareFP<double>(s, exp, 2) );
}

} // namespace tut
