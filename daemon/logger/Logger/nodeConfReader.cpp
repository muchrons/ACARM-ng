/*
 * nodeConfReader.cpp
 *
 */
#include <string>
#include <sstream>
#include <boost/tokenizer.hpp>

#include "ConfigIO/Singleton.hpp"
#include "Logger/nodeConfReader.hpp"

using namespace std;

namespace Logger
{

namespace
{
Priority parsePriority(const string &thr)
{
  if(thr=="debug")
    return Priority::DEBUG;
  // TODO: add rest

  throw ExceptionNoSuchPriority(SYSTEM_SAVE_LOCATION, thr.c_str() );
} // parsePriority()
} // unnamed namespace

NodeConfPtr nodeConfReader(const NodeName &nn, AppenderMap &apps)
{
  const ConfigIO::LoggerConfig     &lc   =ConfigIO::Singleton::get()->loggerConfig();
  const ConfigIO::LoggerNodes       nodes=lc.getNodes();
  const ConfigIO::LoggerNodeConfig &def  =lc.getDefaultNodeConfig();
  Appenders::BasePtr                appender =apps[ def.getAppenderName() ];
  Priority                          threshold=parsePriority( def.getThresholdValue() );

  typedef boost::char_separator<char> Separator;
  typedef boost::tokenizer<Separator> Tokenizer;
  const Separator    sep(".");
  const Tokenizer    tokens(nn.get(), sep);
  stringstream       ss;
  // got through all tokens
//  for(Tokenizer::const_iterator it=tokens.begin(); it!=tokens.end(); ++it)

  // TODO

  return NodeConfPtr( new NodeConf(appender, threshold) );
}

} // namespace Logger
