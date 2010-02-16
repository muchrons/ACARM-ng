/*
 * NodeConfReader.cpp
 *
 */
#include <string>
#include <sstream>
#include <algorithm>
#include <boost/tokenizer.hpp>

#include "Logger/NodeConfReader.hpp"
#include "ConfigIO/Singleton.hpp"

using namespace std;

namespace Logger
{

NodeConfReader::NodeConfReader(void):
  appMap_( ConfigIO::Singleton::get()->loggerConfig().getAppenders() )
{
}

Priority NodeConfReader::parsePriority(const std::string &pri)
{
  if(pri=="debug")
    return Priority::DEBUG;
  if(pri=="info")
    return Priority::INFO;
  if(pri=="warn")
    return Priority::WARN;
  if(pri=="error")
    return Priority::ERROR;
  if(pri=="fatal")
    return Priority::FATAL;

  throw ExceptionNoSuchPriority(SYSTEM_SAVE_LOCATION, pri.c_str() );
}


namespace
{
// functor for finding node object of a given name
struct FindNode
{
  explicit FindNode(const std::string &str):
    str_(&str)
  {
  }
  bool operator()(const ConfigIO::LoggerNodeConfig &cfg) const
  {
    return *str_==cfg.getNodeName();
  }

private:
  const std::string *str_;
}; // struct FindNode
} // unnamed namespace

NodeConfPtr NodeConfReader::read(const NodeName &nn) const
{
  // prepare helper definitions and constants
  const ConfigIO::LoggerConfig     &lc    =ConfigIO::Singleton::get()->loggerConfig();
  const ConfigIO::LoggerNodes      &nodes =lc.getNodes();
  const ConfigIO::LoggerNodeConfig &def   =lc.getDefaultNodeConfig();
  const char                       *sepStr=".";
  // prepare default values for node's configuration parameters
  Appenders::BasePtr                appender =appMap_[ def.getAppenderName() ];
  Priority                          threshold=parsePriority( def.getThresholdValue() );

  typedef boost::char_separator<char> Separator;
  typedef boost::tokenizer<Separator> Tokenizer;
  const Separator    sep(sepStr);
  const Tokenizer    tokens(nn.get(), sep);
  stringstream       ss;
  // got through all tokens (i.e. node names)
  for(Tokenizer::const_iterator it=tokens.begin(); it!=tokens.end(); ++it)
  {
    // create next node path
    if( ss.str().length()>0 )
      ss<<sepStr;
    ss<<*it;
    // check if configuration is present
    ConfigIO::LoggerNodes::const_iterator itCfg=find_if( nodes.begin(),
                                                         nodes.end(),
                                                         FindNode( ss.str() ) );
    if( itCfg==nodes.end() )// if nothing has been found, it means that no more
      break;                // specific conifguration exists

    // change appender, if defined
    if( itCfg->hasAppender() )
      appender =appMap_[ itCfg->getAppenderName() ];
    // change threshold, if defined
    if( itCfg->hasThreshold() )
      threshold=parsePriority( itCfg->getThresholdValue() );
  }

  return NodeConfPtr( new NodeConf(appender, threshold) );
}

} // namespace Logger
