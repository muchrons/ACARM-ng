/*
 * OnProcess.t.cpp
 *
 */
#include <tut.h>
#include <boost/lexical_cast.hpp>

#include "Persistency/Facades/StrAccess/OnProcess.hpp"
#include "Persistency/Facades/StrAccess/TestBaseData.t.hpp"

using namespace Persistency;
using namespace Persistency::Facades::StrAccess;

namespace
{
struct TestClass: public TestBaseData<OnProcess>
{
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
  ensureThrow(*process_, p);
}

// test processing for end element
template<>
template<>
void testObj::test<2>(void)
{
  TestParams p(Path("process.name"), cb_);
  ++p;
  ++p;
  ensureThrow(*process_, p);
}

// test processing for unknown name
template<>
template<>
void testObj::test<3>(void)
{
  TestParams p(Path("service.enemyunknown"), cb_);
  ensureThrow(*process_, p);
}

// test accessing via collection index
template<>
template<>
void testObj::test<4>(void)
{
  ensureProc("failed for colleciton index", *process_, "42.name", "somename");
}

// test getting path
template<>
template<>
void testObj::test<5>(void)
{
  ensureProc("invalid path", *process_, "process.path", "/a/b/c");
}

// test getting path when NULL
template<>
template<>
void testObj::test<6>(void)
{
  ensureProcNull("invalid path", *processNull_, "process.path", "path");
}

// test getting name
template<>
template<>
void testObj::test<7>(void)
{
  ensureProc("invalid name", *process_, "process.name", "somename");
}

// test getting md5
template<>
template<>
void testObj::test<8>(void)
{
  ensureProc("invalid md5", *process_, "process.md5sum", md5_.get());
}

// test getting md5 when NULL
template<>
template<>
void testObj::test<9>(void)
{
  ensureProcNull("invalid md5", *processNull_, "process.md5sum", "md5sum");
}

// test getting pid
template<>
template<>
void testObj::test<10>(void)
{
  ensureProc("invalid pid", *process_, "process.pid", pid_);
}

// test getting pid when NULL
template<>
template<>
void testObj::test<11>(void)
{
  ensureProcNull("invalid pid", *processNull_, "process.pid", "pid");
}

// test getting uid
template<>
template<>
void testObj::test<12>(void)
{
  ensureProc("invalid uid", *process_, "process.uid", uid_);
}

// test getting uid when NULL
template<>
template<>
void testObj::test<13>(void)
{
  ensureProcNull("invalid uid", *processNull_, "process.uid", "uid");
}

// test getting username
template<>
template<>
void testObj::test<14>(void)
{
  ensureProc("invalid username", *process_, "process.username", "satan");
}

// test getting username when NULL
template<>
template<>
void testObj::test<15>(void)
{
  ensureProcNull("invalid username", *processNull_, "process.username", "username");
}

// test getting params
template<>
template<>
void testObj::test<16>(void)
{
  ensureProc("invalid params", *process_, "process.parameters", "-v");
}

// test getting params when NULL
template<>
template<>
void testObj::test<17>(void)
{
  ensureProcNull("invalid params", *processNull_, "process.parameters", "parameters");
}

// test getting reference url
template<>
template<>
void testObj::test<18>(void)
{
  ensureProc("invalid ref url name", *process_, "process.referenceurl.name", url_->getName().get() );
}

// test getting reference url when NULL
template<>
template<>
void testObj::test<19>(void)
{
  ensureProcNull("invalid ref url name", *processNull_, "process.referenceurl.name", "referenceurl");
}

} // namespace tut
