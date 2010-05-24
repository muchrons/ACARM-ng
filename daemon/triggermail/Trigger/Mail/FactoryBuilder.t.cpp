/*
 * FactoryBuilder.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/Mail/FactoryBuilder.hpp"

// TODO

using namespace std;
using namespace Trigger::Mail;

namespace
{

struct TestClass
{
  typedef FactoryBuilder::Options Options;

  void ensureThrow(const Options &opts) const
  {
    bool thrown=false;
    try
    {
      fb_.build(opts);
      thrown=true;
      tut::fail("build() didn't throw on missing/invalid paramter");
    }
    catch(const std::exception&)
    {
      if(thrown)
        throw;
      // this is expected
    }
  }

  void setValid(void)
  {
    /*
    opts_["uin"]                   ="123";
    opts_["password"]              ="secr3t";
    opts_["receiver_uin"]          ="69";
    opts_["severity_threshold"]    ="4.2";
    opts_["alerts_count_threshold"]="42";
    */
  }

  FactoryBuilder fb_;
  Options        opts_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Mail/FactoryBuilder");
} // unnamed namespace


namespace tut
{

//
template<>
template<>
void testObj::test<1>(void)
{
}

//
template<>
template<>
void testObj::test<2>(void)
{
}

//
template<>
template<>
void testObj::test<3>(void)
{
}

//
template<>
template<>
void testObj::test<4>(void)
{
}

//
template<>
template<>
void testObj::test<5>(void)
{
}

//
template<>
template<>
void testObj::test<6>(void)
{
}

//
template<>
template<>
void testObj::test<7>(void)
{
}

//
template<>
template<>
void testObj::test<8>(void)
{
}

//
template<>
template<>
void testObj::test<9>(void)
{
}

//
template<>
template<>
void testObj::test<10>(void)
{
}

//
template<>
template<>
void testObj::test<11>(void)
{
}

//
template<>
template<>
void testObj::test<12>(void)
{
}

} // namespace tut
