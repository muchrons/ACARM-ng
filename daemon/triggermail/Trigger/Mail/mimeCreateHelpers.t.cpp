/*
 * mimeCreateHelpers.t.cpp
 *
 */
#include <tut.h>
#include <cstring>

#include "System/AutoCptr.hpp"
#include "System/ScopedPtrCustom.hpp"
#include "Trigger/Mail/mimeCreateHelpers.hpp"

using namespace System;
using namespace Trigger::Mail;

namespace
{

struct TestClass
{
  AutoCptr<char> cpStr(const char *txt) const
  {
    AutoCptr<char> out( strdup(txt) );
    tut::ensure("NULL-stirng copyied", out.get()!=NULL);
    return out;
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Mail/mimeCreateHelpers");
} // unnamed namespace


namespace tut
{

// smoke test for mem-leaks in buildFields
template<>
template<>
void testObj::test<1>(void)
{
  ScopedPtrCustom<mailimf_fields, mailimf_fields_free> ptr( buildFields("a@b.c", "d@e.f", "re: stuff") );
}

// smoke test for mem-leaks in buildBodyText
template<>
template<>
void testObj::test<2>(void)
{
  ScopedPtrCustom<mailmime, mailmime_free> ptr( buildBodyText("narf") );
}

// smoke test for mem-leaks in buildMessage
template<>
template<>
void testObj::test<3>(void)
{
  ScopedPtrCustom<mailmime, mailmime_free> ptr( buildMessage( buildFields("a@b.c", "d@e.f", "re: stuff") ) );
}

// smoke test for mem-leaks in createMimeMessage
template<>
template<>
void testObj::test<4>(void)
{
  createMimeMessage("a@b.c", "d@e.f", "re: stuff", "hello world");
}

// test mime message content
template<>
template<>
void testObj::test<5>(void)
{
  const std::string mime=createMimeMessage("a@b.c", "d@e.f", "re: stuff", "hello world");
  ensure_equals("invalid mime's content", mime, "");
}

// test for mem-leaks in helper class
template<>
template<>
void testObj::test<6>(void)
{
  const AutoCptr<char> tmp=cpStr("alice ha sa cat");
  ensure("NULL pointer returned", tmp.get()!=NULL);
}

} // namespace tut
