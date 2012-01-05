/*
 * MimeCreateHelpers.t.cpp
 *
 */
#include <tut.h>

#include "System/ScopedPtrCustom.hpp"
#include "Mail/Vmime.hpp"
#include "Mail/MimeCreateHelper.hpp"

using namespace System;
using namespace Mail;

namespace
{

struct TestClass
{
  /* *** EXAMPLE MIME OUTPUT ***
   *
   * Subject: re: stuff
   * From: a@b.c
   * To: d@e.f
   * Date: Mon, 29 Nov 2010 14:46:50 +0100
   * Mime-Version: 1.0
   * Content-Type: text/plain; charset=utf-8
   * Content-Transfer-Encoding: quoted-printable
   *
   * hello world
   *
   */
  void checkMsg(vmime::ref<vmime::message> m) const
  {
    const std::string msg=toString(m);
    tut::ensure("no 'To:' part",           msg.find("To: d@e.f")  !=std::string::npos );
    tut::ensure("no 'From:' part",         msg.find("From: a@b.c")!=std::string::npos );
    tut::ensure("no 'Subject:' part",      msg.find("Subject: re: stuff")!=std::string::npos );
    tut::ensure("no 'Content-Type:' part", msg.find("Content-Type: text/plain; charset=utf-8")!=std::string::npos );
    tut::ensure("no 'body' part",          msg.find("hello world")!=std::string::npos );
  }

  std::string toString(vmime::ref<vmime::message> m) const
  {
    std::stringstream                   ss;
    vmime::utility::outputStreamAdapter out(ss);
    m->generate(out);
    return ss.str();
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Mail/MimeCreateHelper");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  MimeCreateHelper mch("a@b.c", Config::Recipients("d@e.f"), "re: stuff", "hello world");
}

// test content creation
template<>
template<>
void testObj::test<2>(void)
{
  MimeCreateHelper           mch("a@b.c", Config::Recipients("d@e.f"), "re: stuff", "hello world");
  vmime::ref<vmime::message> mime=mch.createMimeMessage();
  checkMsg(mime);
}

// test if multiple calls to createMimeMessage() return the same results
// (this is smoke test for bug in previous versions)
template<>
template<>
void testObj::test<3>(void)
{
  MimeCreateHelper           mch("a@b.c", Config::Recipients("d@e.f"), "re: stuff", "hello world");
  vmime::ref<vmime::message> mime1=mch.createMimeMessage();
  checkMsg(mime1);
  vmime::ref<vmime::message> mime2=mch.createMimeMessage();
  checkMsg(mime2);
}

// test multiple recipients - check structure
template<>
template<>
void testObj::test<4>(void)
{
  Config::Recipients         to("d@e.f");
  to.push_back("r1@d1.org");
  to.push_back("r2@d2.org");
  MimeCreateHelper           mch("a@b.c", to, "re: stuff", "hello world");
  vmime::ref<vmime::message> mime=mch.createMimeMessage();
  checkMsg(mime);
}

// test multiple recipients
template<>
template<>
void testObj::test<5>(void)
{
  Config::Recipients         to("d@e.f");
  to.push_back("r1@d1.org");
  to.push_back("r2@d2.org");
  MimeCreateHelper           mch("a@b.c", to, "re: stuff", "hello world");
  vmime::ref<vmime::message> mime=mch.createMimeMessage();
  const std::string          msg=toString(mime);
  ensure("recipient 1 is missing", msg.find("r1@d1.org")!=std::string::npos );
  ensure("recipient 2 is missing", msg.find("r2@d2.org")!=std::string::npos );
}

// test if exceptions are translated as they should
template<>
template<>
void testObj::test<6>(void)
{
  MimeCreateHelper mch("", Config::Recipients(""), "", "");
  try
  {
    mch.createMimeMessage();
    fail("exception not thrown on empty from/to fields");
  }
  catch(const ExceptionUnableToCreateMessage &)
  {
    // this is expected
  }
}

} // namespace tut
