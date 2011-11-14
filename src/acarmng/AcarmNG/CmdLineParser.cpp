/*
 * CmdLineParser.cpp
 *
 */
#include <boost/program_options.hpp>

#include "AcarmNG/CmdLineParser.hpp"

namespace po = boost::program_options;


namespace AcarmNG
{

CmdLineParser::CmdLineParser(const int argc, char const * const * const argv):
  printHelp_(false),
  printBanner_(true),
  printVersion_(false),
  quitAfterPrint_(false),
  daemonize_(false)
{
  //
  // parse options
  //
  po::options_description desc("Options");
  desc.add_options()
    ("version,v", "show software version and exit")
    ("banner,b", "show program banner and exit")
    ("quiet,q", "do not show anything during startup")
    ("daemonize,d", "daemonize process directly after start")
    ("help,h", "show help message and exit")
  ;

  po::variables_map vm;
  po::store( po::parse_command_line(argc, argv, desc), vm );
  po::notify(vm);

  //
  // process parsed options
  //

  // check quiet mode
  if( vm.count("quiet") )
  {
    printVersion_=false;
    printBanner_ =false;
    printHelp_   =false;
  }

  // check demonization
  if( vm.count("daemonize") )
    daemonize_=true;

  // check version print
  if( vm.count("version") )
  {
    printVersion_  =true;
    printBanner_   =false;
    printHelp_     =false;
    quitAfterPrint_=true;
  }

  // check banner print
  if( vm.count("banner") )
  {
    printVersion_  =false;
    printBanner_   =true;
    printHelp_     =false;
    quitAfterPrint_=true;
  }

  // check help print
  if( vm.count("help") )
  {
    printVersion_  =false;
    printBanner_   =false;
    printHelp_     =true;
    quitAfterPrint_=true;
  }
}

} // namespace AcarmNG
