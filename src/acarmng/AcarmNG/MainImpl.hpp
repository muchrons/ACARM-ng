/*
 * MainImpl.hpp
 *
 */
#ifndef INCLUDE_ACARMNG_MAINIMPL_HPP_FILE
#define INCLUDE_ACARMNG_MAINIMPL_HPP_FILE

#include <string>
#include <boost/noncopyable.hpp>

#include "Logger/Node.hpp"
#include "AcarmNG/CmdLineParser.hpp"
#include "AcarmNG/Exception.hpp"


namespace AcarmNG
{

class MainImpl: private boost::noncopyable
{
public:
  struct ExceptionCannotDaemonize: public Exception
  {
    explicit ExceptionCannotDaemonize(const Location &where);
  };

  MainImpl(int argc, char const * const * argv);

  int run(void);

private:
  void runImpl(void);
  void runApp(void);
  void runAsDaemon(void);

  const Logger::Node  log_;
  const CmdLineParser clp_;
  const std::string   appName_;
}; // struct MainImpl

} // namespace AcarmNG

#endif
