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
  const fs::path now ="loggerfile.log";
  const fs::path prev="loggerfile.log.prev";
  fs::remove(prev);
  fs::rename(now, prev);
  ensure("file not renamed precondition renamed", !fs::exists(now) );
  // send signale to reconfigure
  const int ret=kill( getpid(), SIGHUP );
  ensure_equals("sending signal failed", ret, 0);
  // ensure some logging is done
  LOGMSG_INFO( Logger::Node("core.signaltoreconfigure.tests"), "writing some log file" );
  // check if reconfiguration has been done
  ensure("reconfiguration does not work", fs::exists(now) );
}

} // namespace tut
