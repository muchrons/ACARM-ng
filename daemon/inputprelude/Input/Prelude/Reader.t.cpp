/*
 * Reader.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>
#include <string>

#include "Input/Prelude/Reader.hpp"

using namespace std;
using namespace Input::Prelude;

namespace
{

struct ReaderTestClass
{
  Reader r_;
};

typedef ReaderTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/Prelude/Reader");
} // unnamed namespace


namespace tut
{

// TODO
template<>
template<>
void testObj::test<1>(void)
{
  // TODO
}

} // namespace tut
