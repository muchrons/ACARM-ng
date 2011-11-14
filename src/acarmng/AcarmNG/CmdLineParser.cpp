/*
 * CmdLineParser.cpp
 *
 */
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <ctype.h>
#include <boost/program_options.hpp>

#include "Logger/Logger.hpp"
#include "Commons/Convert.hpp"
#include "AcarmNG/CmdLineParser.hpp"

using namespace std;
namespace po = boost::program_options;


namespace AcarmNG
{

namespace
{

po::options_description makeDesc(void)
{
  po::options_description desc("Options");
  desc.add_options()
    ("version,v",                        "show software version and exit")
    ("banner,b",                         "show program banner and exit")
    ("quiet,q",                          "do not show anything during startup")
    ("daemonize,d",                      "daemonize process directly after start")
    ("user,u",      po::value<string>(), "change UID to this one, before starting")
    ("group,g",     po::value<string>(), "change GID to this one, before starting")
    ("help,h",                           "show help message and exit")
  ;
  return desc;
} // makeDesc()


bool isNumber(const string &str)
{
  if( str.length()<1 )
    return false;
  for(string::const_iterator it=str.begin(); it!=str.end(); ++it)
    if( !isdigit(*it) )
      return false;
  return true;
} // isNumber()

} // unnamed namespace


CmdLineParser::ExceptionUnknownName::ExceptionUnknownName(const Location &where, const char *msg, const std::string &name):
  Exception(where, cc("invalid credentials - ", msg, ": ", name) )
{
}


CmdLineParser::CmdLineParser(const int argc, char const * const * const argv):
  log_("acarmng.cmdlineparser"),
  printHelp_(false),
  printBanner_(true),
  printVersion_(false),
  quitAfterPrint_(false),
  daemonize_(false),
  uid_(0),
  gid_(0)
{
  LOGMSG_DEBUG_S(log_)<<"parsing command line options - got "<<argc<<" arguments (incliuding program name)";

  //
  // parse options
  //
  const po::options_description desc=makeDesc();
  po::variables_map             vm;
  po::store( po::parse_command_line(argc, argv, desc), vm );
  po::notify(vm);

  //
  // process parsed options
  //

  // check quiet mode
  if( vm.count("quiet") )
  {
    LOGMSG_DEBUG(log_, "quiet-print mode enabled");
    printVersion_=false;
    printBanner_ =false;
    printHelp_   =false;
  }

  // check demonization
  if( vm.count("daemonize") )
  {
    LOGMSG_DEBUG(log_, "daemonizing enabled");
    daemonize_=true;
  }

  // check version print
  if( vm.count("version") )
  {
    LOGMSG_DEBUG(log_, "version-print mode enabled");
    printVersion_  =true;
    printBanner_   =false;
    printHelp_     =false;
    quitAfterPrint_=true;
  }

  // check banner print
  if( vm.count("banner") )
  {
    LOGMSG_DEBUG(log_, "banner-print mode enabled");
    printVersion_  =false;
    printBanner_   =true;
    printHelp_     =false;
    quitAfterPrint_=true;
  }

  // check help print
  if( vm.count("help") )
  {
    LOGMSG_DEBUG(log_, "help-print mode enabled");
    printVersion_  =false;
    printBanner_   =false;
    printHelp_     =true;
    quitAfterPrint_=true;
  }

  // parse group
  if( vm.count("group") )
  {
    gid_=getUserID( vm["group"].as<string>() );
    LOGMSG_DEBUG_S(log_)<<"setting group option to "<<gid_;
  }

  // parse user
  if( vm.count("user") )
  {
    uid_=getUserID( vm["user"].as<string>() );
    LOGMSG_DEBUG_S(log_)<<"setting user option to "<<uid_;
  }
}


void CmdLineParser::showHelp(std::ostream &os)
{
  os << makeDesc() << endl;
}


uid_t CmdLineParser::getUserID(const std::string &user) const
{
  LOGMSG_DEBUG_S(log_)<<"mapping user '"<<user<<"' to UID";
  // raw number?
  if( isNumber(user) )
    return Commons::Convert::to<uid_t>(user);
  // need to map name->UID
  struct passwd *u=getpwnam( user.c_str() );
  if(u==NULL)
    throw ExceptionUnknownName(SYSTEM_SAVE_LOCATION, "cannot find user", user);
  assert( user==u->pw_name );
  // all ok
  LOGMSG_INFO_S(log_)<<"user '"<<user<<"' has UID "<<u->pw_uid;
  return u->pw_uid;
}


uid_t CmdLineParser::getGroupID(const std::string &group) const
{
  LOGMSG_DEBUG_S(log_)<<"mapping group '"<<group<<"' to GID";
  // raw number?
  if( isNumber(group) )
    return Commons::Convert::to<gid_t>(group);
  // need to map group->GID
  struct group *g=getgrnam( group.c_str() );
  if(g==NULL)
    throw ExceptionUnknownName(SYSTEM_SAVE_LOCATION, "cannot find group", group);
  assert( group==g->gr_name );
  // all ok
  LOGMSG_INFO_S(log_)<<"group '"<<group<<"' has GID "<<g->gr_gid;
  return g->gr_gid;
}

} // namespace AcarmNG
