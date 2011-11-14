/*
 * CmdLineParser.hpp
 *
 */
#ifndef INCLUDE_ACARMNG_CMDLINEPARSER_HPP_FILE
#define INCLUDE_ACARMNG_CMDLINEPARSER_HPP_FILE

#include <boost/noncopyable.hpp>


namespace AcarmNG
{

struct CmdLineParser: private boost::noncopyable
{
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

private:
  bool printHelp_;
  bool printBanner_;
  bool printVersion_;
  bool quitAfterPrint_;
  bool daemonize_;
}; // struct CmdLineParser

} // namespace AcarmNG

#endif
