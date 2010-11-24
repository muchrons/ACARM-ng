/*
 * SignalToReconfigure.t.cpp
 *
 */
#include <tut.h>
#include <sys/types.h>
#include <signal.h>
#include <cassert>
#include <boost/filesystem.hpp>

#include "Core/SignalToReconfigure.hpp"

using namespace Core;
namespace fs=boost::filesystem;

namespace
{

struct TestClass
{
  TestClass(void):
    sig_(SIGHUP)
  {
  }

  SignalToReconfigure sig_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Core/SignalToReconfigure");
} // unnamed namespace


namespace tut
{

// test reaction on signal
template<>
template<>
void testObj::test<1>(void)
{
  // send proper signal
  const int ret=kill( getpid(), SIGHUP );
  ensure_equals("sending signal failed", ret, 0);
  // TODO: add check if reconfiguration has been called
}

} // namespace tut
