/*
 * OnProcess.t.cpp
 *
 */
#include <tut.h>
#include <boost/lexical_cast.hpp>

#include "Persistency/Facades/StrAccess/OnProcess.hpp"
#include "Persistency/Facades/StrAccess/TestParams.t.hpp"

using namespace Persistency;
using namespace Persistency::Facades::StrAccess;

namespace
{
struct TestClass
{
  TestClass(void):
    md5_( MD5Sum::createFromString("78b9861f74e15d7d0f077ba22421b8e4") ),
    pid_(42),
    uid_(666),
    url_(new ReferenceURL("evil site", "www.evil.info")),
    proc_("/a/b/c", "somename", &md5_, &pid_, &uid_, "satan", "-v", url_),
    procNulls_(NULL, "somename", NULL, NULL, NULL, NULL, NULL, (ReferenceURLPtr()) )
  {
  }

  void ensureThrow(TestParams &p)
  {
    try
    {
      OnProcess::process(proc_, p);
      tut::fail("call didn't throw on invalid path");
    }
    catch(const ExceptionInvalidPath &)
    {
      // this is expected
    }
  }

  template<typename TExp>
  void ensureProc(const char *errMsg, const char *path, const TExp &exp)
  {
    TestParams p(Path(path), cb_);
    OnProcess::process(proc_, p);
    // check
    const std::string expStr=boost::lexical_cast<std::string>(exp);
    tut::ensure_equals(errMsg, cb_.lastValue_, expStr);
  }

  void ensureProcNull(const char *errMsg, const char *path, const std::string &exp)
  {
    TestParams p(Path(path), cb_);
    OnProcess::process(procNulls_, p);
    // check
    tut::ensure_equals(errMsg, cb_.lastNullFound_, exp);
  }

  TestParams::ResultCallback cb_;
  MD5Sum                     md5_;
  pid_t                      pid_;
  int                        uid_;
  ReferenceURLPtr            url_;
  Process                    proc_;
  Process                    procNulls_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/StrAccess/OnProcess");
} // unnamed namespace


namespace tut
{

// test processing last element
template<>
template<>
void testObj::test<1>(void)
{
  TestParams p(Path("process.name"), cb_);
  ++p;
  ensureThrow(p);
}

// test processing for end element
template<>
template<>
void testObj::test<2>(void)
{
  TestParams p(Path("process.name"), cb_);
  ++p;
  ++p;
  ensureThrow(p);
}

// test processing for unknown name
template<>
template<>
void testObj::test<3>(void)
{
  TestParams p(Path("service.enemyunknown"), cb_);
  ensureThrow(p);
}

// test error when invalid root is requested
template<>
template<>
void testObj::test<4>(void)
{
  TestParams p(Path("whatisthat.name"), cb_);
  ensureThrow(p);
}

// test getting path
template<>
template<>
void testObj::test<5>(void)
{
  ensureProc("invalid path", "process.path", "/a/b/c");
}

// test getting path when NULL
template<>
template<>
void testObj::test<6>(void)
{
  ensureProcNull("invalid path", "process.path", "path");
}

// test getting name
template<>
template<>
void testObj::test<7>(void)
{
  ensureProc("invalid name", "process.name", "somename");
}

// test getting md5
template<>
template<>
void testObj::test<8>(void)
{
  ensureProc("invalid md5", "process.md5sum", md5_.get());
}

// test getting md5 when NULL
template<>
template<>
void testObj::test<9>(void)
{
  ensureProcNull("invalid md5", "process.md5sum", "md5sum");
}

// test getting pid
template<>
template<>
void testObj::test<10>(void)
{
  ensureProc("invalid pid", "process.pid", pid_);
}

// test getting pid when NULL
template<>
template<>
void testObj::test<11>(void)
{
  ensureProcNull("invalid pid", "process.pid", "pid");
}

// test getting uid
template<>
template<>
void testObj::test<12>(void)
{
  ensureProc("invalid uid", "process.uid", uid_);
}

// test getting uid when NULL
template<>
template<>
void testObj::test<13>(void)
{
  ensureProcNull("invalid uid", "process.uid", "uid");
}

// test getting username
template<>
template<>
void testObj::test<14>(void)
{
  ensureProc("invalid username", "process.username", "satan");
}

// test getting username when NULL
template<>
template<>
void testObj::test<15>(void)
{
  ensureProcNull("invalid username", "process.username", "username");
}

// test getting params
template<>
template<>
void testObj::test<16>(void)
{
  ensureProc("invalid params", "process.parameters", "-v");
}

// test getting params when NULL
template<>
template<>
void testObj::test<17>(void)
{
  ensureProcNull("invalid params", "process.parameters", "parameters");
}

// test getting reference url
template<>
template<>
void testObj::test<18>(void)
{
  ensureProc("invalid ref url name", "process.referenceurl.name", url_->getName().get() );
}

// test getting reference url when NULL
template<>
template<>
void testObj::test<19>(void)
{
  ensureProcNull("invalid ref url name", "process.referenceurl.name", "referenceurl");
}

} // namespace tut
