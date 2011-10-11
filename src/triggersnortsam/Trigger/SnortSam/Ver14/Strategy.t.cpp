/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/SnortSam/Ver14/Strategy.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Persistency;
using namespace Core::Types::Proc;
using namespace Trigger::SnortSam;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    idAssigner_(new IDAssigner),
    cfg_("127.0.0.1", 4242, "secret", Who::SRC, How::IN, 666, Trigger::Simple::ThresholdConfig("0", "0") ),\
    p_(cfg_, idAssigner_)
  {
  }

  IDAssignerPtrNN             idAssigner_;
  Config                      cfg_;
  Ver14::Strategy::Parameters p_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/SnortSam/Ver14/Strategy");
} // unnamed namespace


namespace tut
{

// smoke-test c-tor
template<>
template<>
void testObj::test<1>(void)
{
  Ver14::Strategy s(InstanceName("ithappens"), p_);
}

} // namespace tut
