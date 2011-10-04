/*
 * NetTCP.t.cpp
 *
 */
#include <tut.h>
#include <boost/asio.hpp>

#include "Trigger/SnortSam/NetTCP.hpp"

using namespace std;
using namespace Trigger::SnortSam;

namespace
{

const char g_req[]="GET / HTTP/1.1\nhost: www.onet.pl\n\n";

struct TestClass
{
  const uint8_t *httpGet(void) const
  {
    return static_cast<const uint8_t*>( static_cast<const void*>(g_req) );
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/SnortSam/NetTCP");
} // unnamed namespace


namespace tut
{

// test sending some data
template<>
template<>
void testObj::test<1>(void)
{
  NetTCP n("onet.pl", 80, 5);
  n.send(httpGet(), sizeof(g_req));
  n.disconnect();
}

// test getting some data
template<>
template<>
void testObj::test<2>(void)
{
  NetTCP n("onet.pl", 80, 5);
  // send request
  n.send(httpGet(), sizeof(g_req));
  // get response (well - at least part)
  const size_t  size=16*1024;
  const DataRef r=n.receive(size);
  ensure_equals("invalid bytes count", r.size(), size);
}

} // namespace tut
