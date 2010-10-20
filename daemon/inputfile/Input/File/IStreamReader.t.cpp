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

// test exception when stream has an error
template<>
template<>
void testObj::test<4>(void)
{
  ss_.setstate(stringstream::badbit);
  try
  {
    isr_.readLine(0u);
    fail("readLine() didn't throw on steram error");
  }
  catch(const Exception &)
  {
    // this is expected
  }
}

// try reading line that is more than buffer size (i.e. 1024)
template<>
template<>
void testObj::test<5>(void)
{
  for(int i=0; i<1024+100; ++i)
    ss_<<i%10;
  ss_<<endl<<"abcdefghijkl"<<endl;
  // read line in 2 steps
  const IStreamReader::Line line=isr_.readLine(0u);
  ensure("nothing has been read", line.first);
  ensure_equals("invalid string len", line.second.length(), 1024u+100u);
}

// try reading part that consists of multiple buffer sizes
template<>
template<>
void testObj::test<6>(void)
{
  for(int i=0; i<3*1024+100; ++i)
    ss_<<i%10;
  ss_<<endl<<"abcdefghijkl"<<endl;
  // read line in 2 steps
  const IStreamReader::Line line=isr_.readLine(0u);
  ensure("nothing has been read", line.first);
  ensure_equals("invalid string len", line.second.length(), 3*1024u+100u);
}

// test if reading multiple lines in one buffer does not break anything
template<>
template<>
void testObj::test<7>(void)
{
  ss_<<"abcde"<<endl;
  ss_<<"fghijk"<<endl;
  // read first line
  {
    const IStreamReader::Line line=isr_.readLine(0u);
    ensure("nothing has been read 1", line.first);
    ensure_equals("invalid string 1", line.second, "abcde");
  }
  // read second line
  {
    const IStreamReader::Line line=isr_.readLine(0u);
    ensure("nothing has been read 2", line.first);
    ensure_equals("invalid string 2", line.second, "fghijk");
  }
}

// test non-zero timeout
template<>
template<>
void testObj::test<8>(void)
{
  System::Timer t;
  ensure("something has been read", isr_.readLine(1u).first==false );
  ensure("timeout is too small", t.elapsed()>=1.0 );    // minimum 1[s]
  ensure("timeout is too long",  t.elapsed()< 2.0 );    // less then 2x deadline
}

// test if reading small lines does not trigger reading from stream, if data is already buffered
template<>
template<>
void testObj::test<9>(void)
{
  ss_<<"ab"<<endl;
  ss_<<"cd"<<endl;
  ensure_equals("stream already read?!", ss_.tellg(), 0u);
  // read first line - data is being buffered here
  {
    const IStreamReader::Line line=isr_.readLine(0u);
    ensure("nothing has been read 1", line.first);
    ensure_equals("invalid string 1", line.second, "ab");
  }
  ensure_equals("stream read to invalid point", ss_.tellg(), 6u);

  ss_<<"ef"<<endl;
  ss_<<"gh"<<endl;
  // read second line - no data buffering should be done
  {
    const IStreamReader::Line line=isr_.readLine(0u);
    ensure("nothing has been read 2", line.first);
    ensure_equals("invalid string 2", line.second, "cd");
  }
  ensure_equals("stream has been read without a point", ss_.tellg(), 6u);
}

} // namespace tut
