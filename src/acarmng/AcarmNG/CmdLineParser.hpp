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

// TODO: When invalid option is used, usage should be printed.
// TODO: When the same option is used twice, usage should be printed.
namespace AcarmNG
{

/** \brief class performming parsing of the command line arguments.
 *
 *  arguments are parsed once and results are saved locally, for later usage.
 */
struct CmdLineParser: private boost::noncopyable
{
  /** \brief exception thrown when unknown user/group name is passed.
   */
  struct ExceptionUnknownName: public Exception
  {
    /** \brief create exception message/type.
     *  \param where location where exception has been risen.
     *  \param msg   exact message (what exactly went wrong).
     *  \param name  name that caused error.
     */
    ExceptionUnknownName(const Location &where, const char *msg, const std::string &name);
  }; // struct ExceptionUnknownName


  /** \brief parse command line.
   *  \param argc number of arguments.
   *  \param argv table of arguments.
   */
  CmdLineParser(int argc, char const * const * argv);

  /** \brief gets information if help should be printed.
   */
  bool printHelp(void) const
  {
    return printHelp_;
  }
  /** \brief gets info if banner should be printed.
   */
  bool printBanner(void) const
  {
    return printBanner_;
  }
  /** \brief gets info if version information should be printed.
   */
  bool printVersion(void) const
  {
    return printVersion_;
  }
  /** \brief informs if program should quit after printing required pieces of information.
   */
  bool quitAfterPrint(void) const
  {
    return quitAfterPrint_;
  }
  /** \brief informs if process should demonize before running actuall code.
   */
  bool daemonize(void) const
  {
    return daemonize_;
  }
  /** \brief gets UID to change to before running.
   */
  uid_t userID(void) const
  {
    return uid_;
  }
  /** \brief gets GID to change to before running.
   */
  gid_t groupID(void) const
  {
    return gid_;
  }

  /** \brief prints help message to a given stream.
   */
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
