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
  const char *mimeOutput="Date: Mon, 31 May 2010 10:58:50 +0200\r\n"
                         "From: a@b.c\r\n"
                         "To: d@e.f\r\n"
                         "Message-ID: <etPan.4c037a4a.643c9869.4eab@pinky>\r\n"
                         "Subject: re: stuff\r\n"
                         "MIME-Version: 1.0\r\n"
                         "Content-Type: text/plain; charset=\"utf-8\"\r\n"
                         "Content-Transfer-Encoding: 8bit\r\n"
                         "\r\n"
                         "hello world.\r\n";

  MimeCreateHelper  mch("a@b.c", "d@e.f", "re: stuff", "hello world");
  const std::string mime=mch.createMimeMessage();
  ensure_equals("invalid mime's content", mime, mimeOutput);
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
