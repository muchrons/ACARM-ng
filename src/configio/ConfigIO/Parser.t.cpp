/*
 * Parser.t.cpp
 *
 */
#include <tut.h>

#include "ConfigIO/Parser.hpp"

using namespace std;
using namespace ConfigIO;

namespace
{
struct ParserTestClass
{
};

typedef ParserTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/Parser");
} // unnamed namespace


namespace tut
{

// 
template<>
template<>
void testObj::test<1>(void)
{
  Parser p("aa");
}

// 
template<>
template<>
void testObj::test<2>(void)
{
}

// 
template<>
template<>
void testObj::test<3>(void)
{
}

} // namespace tut
