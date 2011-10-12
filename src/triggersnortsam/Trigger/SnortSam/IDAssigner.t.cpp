/*
 * IDAssigner.t.cpp
 *
 */
#include <tut.h>
#include <memory>

#include "Trigger/SnortSam/IDAssigner.hpp"
#include "TestHelpers/Persistency/ConnectionIOMemory.hpp"

using namespace Trigger::SnortSam;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass
{
  TestClass(void):
    ptr_(new ConnectionIOMemory),
    conn_(ptr_),
    bf_( conn_, Core::Types::Proc::TypeName("test_type"), Core::Types::Proc::InstanceName("test_instance") ),
    ida_(new IDAssigner)
  {
  }

  ConnectionIOMemory               *ptr_;
  Persistency::IO::ConnectionPtrNN  conn_;
  Trigger::BackendFacade            bf_;
  std::auto_ptr<IDAssigner>         ida_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/SnortSam/IDAssigner");
} // unnamed namespace


namespace tut
{

// test createing smart pointer
template<>
template<>
void testObj::test<1>(void)
{
  IDAssignerPtrNN tmp( ida_.release() );
}

// test assigning new value
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invalid first value assigned", ida_->assign(bf_), 1u);
}

// test assigning two values, one by one
template<>
template<>
void testObj::test<3>(void)
{
  const unsigned int id1=ida_->assign(bf_);
  const unsigned int id2=ida_->assign(bf_);
  ensure("invalid elements order", id1<id2);
}

// test assigning IDs by different IDAssigner instances
template<>
template<>
void testObj::test<4>(void)
{
  const unsigned int id1=ida_->assign(bf_);
  ida_.reset();
  std::auto_ptr<IDAssigner> tmp(new IDAssigner);
  const unsigned int id2=tmp->assign(bf_);
  ensure("invalid elements order after re-init", id1<id2);
}

// test reading initial value
template<>
template<>
void testObj::test<5>(void)
{
  IODynamicConfigMemory::Memory &mem=ptr_->otm_["Filter::SnortSam"];
  mem["next free ID"]="42";
  ensure_equals("invalid value read", ida_->assign(bf_), 42u);
}

// test writing value to persistent storage
template<>
template<>
void testObj::test<6>(void)
{
  IODynamicConfigMemory::Memory &mem=ptr_->otm_["Filter::SnortSam"];
  mem["next free ID"]="41";
  ensure_equals("pre-condition failed", ida_->assign(bf_), 41u);
  ensure_equals("invalid value saved", mem["next free ID"], "42");
}

} // namespace tut
