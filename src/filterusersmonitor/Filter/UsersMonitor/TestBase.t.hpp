/*
 * TestBase.t.hpp
 *
 */
#ifndef INCLUDE_FILTER_USERSMONITOR_TESTBASE_T_HPP_FILE
#define INCLUDE_FILTER_USERSMONITOR_TESTBASE_T_HPP_FILE

#include "Persistency/Alert.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

namespace
{

struct TestBase: public TestHelpers::Persistency::TestStubs
{
  Persistency::ConstAlertPtrNN mkAlert(const char *src1=NULL, const char *src2=NULL,
                                       const char *dst1=NULL, const char *dst2=NULL,
                                       const bool  force=false) const
  {
    Persistency::Alert::Hosts src;
    src.push_back( mkHost(src1, src2, force) );
    Persistency::Alert::Hosts tgt;
    tgt.push_back( mkHost(dst1, dst2, force) );

    return Persistency::ConstAlertPtrNN( new Persistency::Alert("n",
                                                                Persistency::Alert::Analyzers( TestHelpers::Persistency::makeNewAnalyzer() ),
                                                                NULL, Persistency::Timestamp(),
                                                                Persistency::Severity(Persistency::SeverityLevel::INFO),
                                                                0.42, "d", src, tgt) );
  }

  Persistency::HostPtrNN mkHost(const char *user1, const char *user2, bool force) const
  {
    Persistency::Host::Processes p;
    if(user1!=NULL || force)
      p.push_back( mkProc(user1) );
    if(user2!=NULL || force)
      p.push_back( mkProc(user2) );
    return Persistency::HostPtrNN( new Persistency::Host(Persistency::Host::IPv4::from_string("1.2.3.4"),
                                                         NULL, "linux",
                                                         Persistency::ConstReferenceURLPtr(),
                                                         Persistency::Host::Services(), p, NULL) );
  }

  Persistency::ConstProcessPtrNN mkProc(const char *user) const
  {
    return Persistency::ProcessPtrNN( new Persistency::Process("/a/b", "b", NULL, NULL, NULL, user, NULL,
                                                               Persistency::ConstReferenceURLPtr()) );
  }
}; // struct TestBase

} // unnamed namespace

#endif
