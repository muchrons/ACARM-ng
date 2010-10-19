/*
 * IStreamReader.t.cpp
 *
 */
#include <tut.h>

#include "Input/File/IStreamReader.hpp"
#include "System/Timer.hpp"

using namespace std;
using namespace Input::File;

namespace
{

struct TestClass
{
  TestClass(void):
    isr_(ss_)
  {
  }

  void seek(void)
  {
    // move to the begining of the stream - without this "feature" stringstream
    // gives more-less random results when reading with readsome(). dont ask me...
    ss_.seekg(0, stringstream::beg);
  }

  stringstream  ss_;
  IStreamReader isr_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/File/IStreamReader");
} // unnamed namespace


namespace tut
{

// test reading when noting is written
template<>
template<>
void testObj::test<1>(void)
{
  ensure("somethign has been read", isr_.readLine(0u).first==false );
}

// test if read does not return anything if no new line is found
template<>
template<>
void testObj::test<2>(void)
{
  ss_<<"some string without new line";
  seek();
  ensure("somethign has been read", isr_.readLine(0u).first==false );
}

// test if new line has been added
template<>
template<>
void testObj::test<3>(void)
{
  ss_<<"some string"<<endl;
  seek();
  const IStreamReader::Line line=isr_.readLine(0u);
  ensure("nothing has been read", line.first);
  ensure_equals("invalid string", line.second, "some string");
}

// 
template<>
template<>
void testObj::test<4>(void)
{
}

// 
template<>
template<>
void testObj::test<5>(void)
{
}

// 
template<>
template<>
void testObj::test<6>(void)
{
}

// 
template<>
template<>
void testObj::test<7>(void)
{
}

// 
template<>
template<>
void testObj::test<8>(void)
{
}

// 
template<>
template<>
void testObj::test<9>(void)
{
}

// 
template<>
template<>
void testObj::test<10>(void)
{
}

// 
template<>
template<>
void testObj::test<11>(void)
{
}

// 
template<>
template<>
void testObj::test<12>(void)
{
}

} // namespace tut
