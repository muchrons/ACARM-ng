/*
 * MailSmtp.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/Mail/MailSmtp.hpp"

using namespace Trigger::Mail;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Mail/MailSmtp");
} // unnamed namespace


namespace tut
{

// test creating instance
template<>
template<>
void testObj::test<1>(void)
{
  MailSmtp ms;
  ensure("NULL pointer returned", ms.get()!=NULL );
}

// test creating multiple instances
template<>
template<>
void testObj::test<2>(void)
{
  MailSmtp ms1;
  MailSmtp ms2;
}

// test get()-const
template<>
template<>
void testObj::test<3>(void)
{
  MailSmtp        ms1;
  const MailSmtp &ms2=ms1;
  ensure("get() and get()-const differ in result", ms1.get()==ms2.get() );
}

} // namespace tut
