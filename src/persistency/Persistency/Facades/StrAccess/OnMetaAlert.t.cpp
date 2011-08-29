/*
 * OnMetaAlert.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/Facades/StrAccess/OnMetaAlert.hpp"
#include "Persistency/Facades/StrAccess/TestBaseData.t.hpp"

using namespace Persistency;
using namespace Persistency::Facades::StrAccess;

namespace
{
struct TestClass: public TestBaseData<OnMetaAlert>
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/StrAccess/OnMetaAlert");
} // unnamed namespace


namespace tut
{

// test getting name
template<>
template<>
void testObj::test<1>(void)
{
  ensureProc("invalid name", *metaalert_, "metaalert.name", "meta alert name");
}

// test getting id
template<>
template<>
void testObj::test<2>(void)
{
  ensureProc("invalid id", *metaalert_, "metaalert.id", "303");
}

// test processing last element
template<>
template<>
void testObj::test<3>(void)
{
  TestParams p(Path("metaalert.name"), cb_);
  ++p;
  ensureThrow(*metaalert_, p);
}

// test processing for end element
template<>
template<>
void testObj::test<4>(void)
{
  TestParams p(Path("metaalert.name"), cb_);
  ++p;
  ++p;
  ensureThrow(*metaalert_, p);
}

// test processing for unknown name
template<>
template<>
void testObj::test<5>(void)
{
  TestParams p(Path("metaalert.enemyunknown"), cb_);
  ensureThrow(*metaalert_, p);
}

// test getting severity delta
template<>
template<>
void testObj::test<6>(void)
{
  ensureProc("invalid severity delta", *metaalert_, "metaalert.severitydelta", "0.5");
}

// test getting certanity delta
template<>
template<>
void testObj::test<7>(void)
{
  ensureProc("invalid certainty delta", *metaalert_, "metaalert.certaintydelta", "-0.5");
}

// test getting creation time
template<>
template<>
void testObj::test<8>(void)
{
  ensureProc("invalid creation time", *metaalert_, "metaalert.created", "556677");
}

// test getting reference url
template<>
template<>
void testObj::test<9>(void)
{
  ensureProc("invalid ref. ulr", *metaalert_, "metaalert.referenceurl.name", "evil site");
}

// test children count when leaf
template<>
template<>
void testObj::test<10>(void)
{
  TestParams p(Path("metaalert.children.size"), cb_);
  ensureThrowEx<ExceptionNotNode>("call didn't thrown on getting size of leaf's children", *metaalertLeaf_, p);
}

// test children count when node
template<>
template<>
void testObj::test<11>(void)
{
  ensureProcSize("invalid children count for node", *metaalertNode_, "metaalert.children.size", 2);
}

// test accessing random child
template<>
template<>
void testObj::test<12>(void)
{
  ensureProc("invalid child's id", *metaalertNode_, "metaalert.children.1.id", "102");
}

// test accessing alert's data (when leaf)
template<>
template<>
void testObj::test<13>(void)
{
  ensureProc("invalid alert's name", *metaalertLeaf_, "metaalert.alert.name", "abc");
}

// test accessing alert's data (when node)
template<>
template<>
void testObj::test<14>(void)
{
  TestParams p(Path("metaalert.alert.name"), cb_);
  ensureThrowEx<ExceptionNotLeaf>("call didn't throw on leaf-like accessing of node", *metaalertNode_, p);
}

// test getting reference url when NULL
template<>
template<>
void testObj::test<15>(void)
{
  ensureProcNull("invalid ref. url when NULL", *metaalertNull_, "metaalert.referenceurl.name", "referenceurl");
}

// test is leaf is a leaf
template<>
template<>
void testObj::test<16>(void)
{
  ensureProc("leaf is not a leaf", *metaalertLeaf_, "metaalert.isleaf", "true");
}

// test is node is a leaf
template<>
template<>
void testObj::test<17>(void)
{
  ensureProc("node is a leaf", *metaalertNode_, "metaalert.isleaf", "false");
}

// test when accessed as a part of the collection
template<>
template<>
void testObj::test<18>(void)
{
  ensureProc("cannot access as a coleciton element", *metaalert_, "42.name", "meta alert name");
}

// test acess via invalid root name
template<>
template<>
void testObj::test<19>(void)
{
  TestParams p(Path("invalidroot.name"), cb_);
  ensureThrow(*metaalert_, p);
}

// test accessing some element down below
template<>
template<>
void testObj::test<20>(void)
{
  ensureProc("cannot access bottom element", *metaalertNode_, "metaalert.children.1.alert.analyzers.0.name", "some analyzer");
}

} // namespace tut
