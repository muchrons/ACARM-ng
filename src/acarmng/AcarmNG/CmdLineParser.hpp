/*
 * CmdLineParser.hpp
 *
 */
#ifndef INCLUDE_ACARMNG_CMDLINEPARSER_HPP_FILE
#define INCLUDE_ACARMNG_CMDLINEPARSER_HPP_FILE

#include <iosfwd>
#include <pwd.h>
#include <grp.h>
#include <boost/noncopyable.hpp>

#include "Logger/Node.hpp"
#include "AcarmNG/Exception.hpp"


namespace AcarmNG
{

struct CmdLineParser: private boost::noncopyable
{
  struct ExceptionUnknownName: public Exception
  {
    ExceptionUnknownName(const Location &where, const char *msg, const std::string &name);
  }; // struct ExceptionUnknownName


  CmdLineParser(int argc, char const * const * argv);

  bool printHelp(void) const
  {
    return printHelp_;
  }
  bool printBanner(void) const
  {
    return printBanner_;
  }
  bool printVersion(void) const
  {
    return printVersion_;
  }
  bool quitAfterPrint(void) const
  {
    return quitAfterPrint_;
  }
  bool daemonize(void) const
  {
    return daemonize_;
  }
  uid_t userID(void) const
  {
    return uid_;
  }
  gid_t groupID(void) const
  {
    return gid_;
  }

  static void showHelp(std::ostream &os);

private:
  uid_t getUserID(const std::string &user) const;
  uid_t getGroupID(const std::string &group) const;

  Logger::Node log_;
  bool         printHelp_;
  bool         printBanner_;
  bool         printVersion_;
  bool         quitAfterPrint_;
  bool         daemonize_;
  uid_t        uid_;
  gid_t        gid_;
}; // struct CmdLineParser

} // namespace AcarmNG

#endif
