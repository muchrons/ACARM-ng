/*
 * MimeCreateHelpers.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cstring>

#include "System/ScopedPtrCustom.hpp"
#include "Trigger/Mail/MimeCreateHelper.hpp"

using namespace System;
using namespace Trigger::Mail;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Mail/MimeCreateHelper");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  MimeCreateHelper mch("a@b.c", "d@e.f", "re: stuff", "hello world");
}

// test content creation
template<>
template<>
void testObj::test<2>(void)
{
  MimeCreateHelper  mch("a@b.c", "d@e.f", "re: stuff", "hello world");
  const std::string mime=mch.createMimeMessage();
  ensure_equals("invalid mime's content", mime, "");
}

// test if multiple calls to createMimeMessage() return the same results
template<>
template<>
void testObj::test<3>(void)
{
  MimeCreateHelper  mch("a@b.c", "d@e.f", "re: stuff", "hello world");
  const std::string mime1=mch.createMimeMessage();
  const std::string mime2=mch.createMimeMessage();
  ensure_equals("different strings returned", mime1, mime2);
}

} // namespace tut
