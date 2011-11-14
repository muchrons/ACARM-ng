/*
 * CmdLineParser.t.cpp
 *
 */
#include <tut.h>
#include <sstream>

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

} // namespace tut
