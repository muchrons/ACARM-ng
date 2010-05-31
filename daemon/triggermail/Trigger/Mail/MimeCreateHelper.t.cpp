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
  /* *** EXAMPLE MIME OUTPUT ***
   *
   * Date: Mon, 31 May 2010 10:58:50 +0200
   * From: a@b.c
   * To: d@e.f
   * Message-ID: <etPan.4c037a4a.643c9869.4eab@pinky>
   * Subject: re: stuff
   * MIME-Version: 1.0
   * Content-Type: text/plain; charset="utf-8"
   * Content-Transfer-Encoding: 8bit
   *
   * hello world.
   *
   */
  void checkMsg(const std::string &msg) const
  {
    tut::ensure("no 'To:' part",           msg.find("To: d@e.f")  !=std::string::npos );
    tut::ensure("no 'From:' part",         msg.find("From: a@b.c")!=std::string::npos );
    tut::ensure("no 'Subject:' part",      msg.find("Subject: re: stuff")!=std::string::npos );
    tut::ensure("no 'Content-Type:' part", msg.find("Content-Type: text/plain; charset=\"utf-8\"")!=std::string::npos );
    tut::ensure("no 'body' part",          msg.find("hello world")!=std::string::npos );
  }
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
  checkMsg(mime);
}

// test if multiple calls to createMimeMessage() return the same results
// (this is smoke test for bug in previous versions)
template<>
template<>
void testObj::test<3>(void)
{
  MimeCreateHelper  mch("a@b.c", "d@e.f", "re: stuff", "hello world");
  const std::string mime1=mch.createMimeMessage();
  checkMsg(mime1);
  const std::string mime2=mch.createMimeMessage();
  checkMsg(mime2);
}

} // namespace tut
