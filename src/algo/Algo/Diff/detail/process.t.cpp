/*
 * process.t.cpp
 *
 */
#include <tut.h>

#include "Algo/Diff/detail/process.hpp"

#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace ::Persistency;
using namespace Algo::Diff::detail;
using Algo::Diff::Similarity;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/Diff/detail/process");
} // unnamed namespace


namespace tut
{

// check if identical processs are the same
template<>
template<>
void testObj::test<1>(void)
{
  const MD5Sum          md5=MD5Sum::createFromString("01234567890123456789012345678901");
  const ReferenceURLPtr url=makeNewReferenceURL("url").shared_ptr();
  const pid_t           pid=42;
  const int             uid=50;
  const Process p1("/a/b/c", "c", &md5, &pid, &uid, "kiler", "abc", url);
  const Process p2("/a/b/c", "c", &md5, &pid, &uid, "kiler", "abc", url);
  const Similarity s=compare(p1, p2);
  ensure("identical elements differ", s.get()>0.99);
}

// test slightly different processs
template<>
template<>
void testObj::test<2>(void)
{
  const Process p1("/a/b/c", "c", NULL, NULL, NULL, "kiler", "abc",  ReferenceURLPtr() );
  const Process p2("/a/c/z", "D", NULL, NULL, NULL, "kiler", "QWER", ReferenceURLPtr() );
  const Similarity s=compare(p1, p2);
  ensure("elements differ too little", s.get()>0.5);
  ensure("elements marked identical", s.get()<1);
}

// test totally different processs
template<>
template<>
void testObj::test<3>(void)
{
  const MD5Sum          md5=MD5Sum::createFromString("01234567890123456789012345678901");
  const ReferenceURLPtr url=makeNewReferenceURL("url").shared_ptr();
  const pid_t           pid=42;
  const int             uid=50;
  const Process p1("/some/path", "c", &md5, &pid, &uid, "kiler",  "abc", url);
  const Process p2("/other/dir", "x", NULL, NULL, NULL, "stefan", "xyz", ReferenceURLPtr() );
  const Similarity s=compare(p1, p2);
  ensure("elements differ too little", s.get()<0.1);
}

} // namespace tut
