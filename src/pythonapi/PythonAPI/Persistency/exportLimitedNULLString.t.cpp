/*
 * exportLimitedNULLString.t.cpp
 *
 */
#include <tut.h>

#include "PythonAPI/Persistency/exportLimitedNULLString.hpp"

using namespace std;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("PythonAPI/Persistency/exportLimitedNULLString");
} // unnamed namespace

namespace tut
{

// try preparing test string for export
template<>
template<>
void testObj::test<1>(void)
{
  PythonAPI::Persistency::exportLimitedNULLString<42>("test");
}

} // namespace tut
