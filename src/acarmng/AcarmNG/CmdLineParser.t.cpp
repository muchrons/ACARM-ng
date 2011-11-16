/*
 * CmdLineParser.t.cpp
 *
 */
#include <tut.h>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>

#include "AcarmNG/CmdLineParser.hpp"

using namespace std;
using namespace AcarmNG;

namespace
{

struct TestClass
{
  void check(const CmdLineParser &clp, const bool help,
                                       const bool banner,
                                       const bool version,
                                       const bool quit,
                                       const bool daemonize) const
  {
    tut::ensure_equals("invalid help",             clp.printHelp(),      help);
    tut::ensure_equals("invalid banner",           clp.printBanner(),    banner);
    tut::ensure_equals("invalid version",          clp.printVersion(),   version);
    tut::ensure_equals("invalid quit-after-print", clp.quitAfterPrint(), quit);
    tut::ensure_equals("invalid daemonize",        clp.daemonize(),      daemonize);
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("AcarmNG/CmdLineParser");
} // unnamed namespace


namespace tut
{

// test help -- long
template<>
template<>
void testObj::test<1>(void)
{
  const int          argc  =2;
  const char * const argv[]={"./a.out", "--help"};
  check( CmdLineParser(argc, argv), true, false, false, true, false);
}

// test help -- short
template<>
template<>
void testObj::test<2>(void)
{
  const int          argc  =2;
  const char * const argv[]={"./a.out", "-h"};
  check( CmdLineParser(argc, argv), true, false, false, true, false);
}

// test banner -- long
template<>
template<>
void testObj::test<3>(void)
{
  const int          argc  =2;
  const char * const argv[]={"./a.out", "--banner"};
  check( CmdLineParser(argc, argv), false, true, false, true, false);
}

// test banner -- short
template<>
template<>
void testObj::test<4>(void)
{
  const int          argc  =2;
  const char * const argv[]={"./a.out", "-b"};
  check( CmdLineParser(argc, argv), false, true, false, true, false);
}

// test version -- long
template<>
template<>
void testObj::test<5>(void)
{
  const int          argc  =2;
  const char * const argv[]={"./a.out", "--version"};
  check( CmdLineParser(argc, argv), false, false, true, true, false);
}

// test version -- short
template<>
template<>
void testObj::test<6>(void)
{
  const int          argc  =2;
  const char * const argv[]={"./a.out", "-v"};
  check( CmdLineParser(argc, argv), false, false, true, true, false);
}

// test quiet -- long
template<>
template<>
void testObj::test<7>(void)
{
  const int          argc  =2;
  const char * const argv[]={"./a.out", "--quiet"};
  check( CmdLineParser(argc, argv), false, false, false, false, false);
}

// test quiet -- short
template<>
template<>
void testObj::test<8>(void)
{
  const int          argc  =2;
  const char * const argv[]={"./a.out", "-q"};
  check( CmdLineParser(argc, argv), false, false, false, false, false);
}

// test daemonize -- long
template<>
template<>
void testObj::test<9>(void)
{
  const int          argc  =2;
  const char * const argv[]={"./a.out", "--daemonize"};
  check( CmdLineParser(argc, argv), false, true, false, false, true);
}

// test daemonize -- short
template<>
template<>
void testObj::test<10>(void)
{
  const int          argc  =2;
  const char * const argv[]={"./a.out", "-d"};
  check( CmdLineParser(argc, argv), false, true, false, false, true);
}

// test quiet deamonization
template<>
template<>
void testObj::test<11>(void)
{
  const int          argc  =3;
  const char * const argv[]={"./a.out", "--daemonize", "--quiet"};
  check( CmdLineParser(argc, argv), false, false, false, false, true);
}

// test default options
template<>
template<>
void testObj::test<12>(void)
{
  const int          argc  =1;
  const char * const argv[]={"./a.out"};
  check( CmdLineParser(argc, argv), false, true, false, false, false);
}

// test showing help screen
template<>
template<>
void testObj::test<13>(void)
{
  std::stringstream  ss;
  CmdLineParser::showHelp(ss);
  const std::string &str=ss.str();
  // check some random elements
  ensure("no '--help'",   str.find("--help")  !=std::string::npos);
  ensure("no '--daemon'", str.find("--daemon")!=std::string::npos);
}

// test default UID/GID (i.e. current group/user)
template<>
template<>
void testObj::test<14>(void)
{
  const int           argc  =1;
  const char * const  argv[]={"./a.out"};
  const CmdLineParser clp(argc, argv);
  ensure_equals("invalid UID", clp.userID(),  getuid() );
  ensure_equals("invalid GID", clp.groupID(), getgid() );
}

// test getting UID from UID -- long
template<>
template<>
void testObj::test<15>(void)
{
  const int           argc  =3;
  const char * const  argv[]={"./a.out", "--user", "666"};
  const CmdLineParser clp(argc, argv);
  ensure_equals("invalid UID", clp.userID(), 666);
}

// test getting UID from UID -- short
template<>
template<>
void testObj::test<16>(void)
{
  const int           argc  =3;
  const char * const  argv[]={"./a.out", "-u", "666"};
  const CmdLineParser clp(argc, argv);
  ensure_equals("invalid UID", clp.userID(), 666);
}

// test getting UID from name
template<>
template<>
void testObj::test<17>(void)
{
  const int           argc  =3;
  const char * const  argv[]={"./a.out", "--user", "root"};
  const CmdLineParser clp(argc, argv);
  ensure_equals("invalid UID", clp.userID(), 0);
}

// test getting GID from GID -- long
template<>
template<>
void testObj::test<18>(void)
{
  const int          argc  =3;
  const char * const argv[]={"./a.out", "--group", "666"};
  const CmdLineParser clp(argc, argv);
  ensure_equals("invalid UID", clp.groupID(), 666);
}

// test getting GID from GID -- short
template<>
template<>
void testObj::test<19>(void)
{
  const int          argc  =3;
  const char * const argv[]={"./a.out", "-g", "666"};
  const CmdLineParser clp(argc, argv);
  ensure_equals("invalid UID", clp.groupID(), 666);
}

// test getting GID from name
template<>
template<>
void testObj::test<20>(void)
{
  const int          argc  =3;
  const char * const argv[]={"./a.out", "--group", "root"};
  const CmdLineParser clp(argc, argv);
  ensure_equals("invalid UID", clp.groupID(), 0);
}

// test error message on unknown group
template<>
template<>
void testObj::test<21>(void)
{
  const int          argc  =3;
  const char * const argv[]={"./a.out", "--group", "thisgroupdoesnotexist"};
  try
  {
    CmdLineParser clp(argc, argv);
    fail("parsing of unknown group didn't failed");
  }
  catch(const CmdLineParser::ExceptionUnknownName&)
  {
    // this is expected
  }
}

// test error message on negative GID
template<>
template<>
void testObj::test<22>(void)
{
  const int          argc  =3;
  const char * const argv[]={"./a.out", "--group", "-666"};
  try
  {
    CmdLineParser clp(argc, argv);
    fail("parsing of negative GID didn't failed");
  }
  catch(const CmdLineParser::ExceptionUnknownName&)
  {
    // this is expected
  }
}

// test error message on unknown user
template<>
template<>
void testObj::test<23>(void)
{
  const int          argc  =3;
  const char * const argv[]={"./a.out", "--user", "thisuserdoesnotexist"};
  try
  {
    CmdLineParser clp(argc, argv);
    fail("parsing of unknown user didn't failed");
  }
  catch(const CmdLineParser::ExceptionUnknownName&)
  {
    // this is expected
  }
}

// test error message on negative UID
template<>
template<>
void testObj::test<24>(void)
{
  const int          argc  =3;
  const char * const argv[]={"./a.out", "--user", "-666"};
  try
  {
    CmdLineParser clp(argc, argv);
    fail("parsing of negiative UID didn't failed");
  }
  catch(const CmdLineParser::ExceptionUnknownName&)
  {
    // this is expected
  }
}

// TODO: test case for invalid command line option should be added - if mekes sense

} // namespace tut
