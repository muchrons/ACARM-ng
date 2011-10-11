/*
 * NoNetworkIO.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <vector>
#include <utility>
#include <ctime>
#include <boost/asio.hpp>

#include "Trigger/SnortSam/NoNetworkIO.hpp"

using namespace std;
using namespace Trigger::SnortSam;

namespace
{

struct TestClass
{
  NoNetworkIO nnio_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/SnortSam/NoNetworkIO");
} // unnamed namespace


namespace tut
{

// test if sending allows receiving the same data
template<>
template<>
void testObj::test<1>(void)
{
  const uint8_t buf[]={1,2,3};
  nnio_.send(buf, sizeof(buf));
  const DataRef r=nnio_.receive(sizeof(buf));
  // check
  ensure_equals("invalid size", r.size(), sizeof(buf));
  for(size_t i=0; i<sizeof(buf); ++i)
    ensure_equals("invalid element", r[i], buf[i]);
}

// test if sending owerwrites what's saved from previous call
template<>
template<>
void testObj::test<2>(void)
{
  // first send
  {
    const uint8_t buf[]={1,2,3};
    nnio_.send(buf, sizeof(buf));
  }
  // second send
  const uint8_t buf[]={1,2};
  nnio_.send(buf, sizeof(buf));
  // read
  const DataRef r=nnio_.receive(sizeof(buf));
  // check
  ensure_equals("invalid size", r.size(), sizeof(buf));
  for(size_t i=0; i<sizeof(buf); ++i)
    ensure_equals("invalid element", r[i], buf[i]);
}

} // namespace tut
