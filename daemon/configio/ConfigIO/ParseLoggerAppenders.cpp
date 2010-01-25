/*
 * ParseLoggerAppenders.cpp
 *
 */
#include <cassert>

#include "ConfigIO/ParseLoggerAppenders.hpp"

using namespace std;
using namespace XML;

namespace ConfigIO
{

ParseLoggerAppenders::ParseLoggerAppenders(const XML::Node &node):
  appenders_( parse(node) )
{
}


LoggerAppenders ParseLoggerAppenders::parse(const XML::Node &node) const
{
  // this in fact will be already checked before this call happens
  assert(node.getName()=="appenders");

  LoggerAppenders::Appenders  apps;
  const Node::TNodesList     &children=node.getChildrenList();

  // at least one appender is required (default appender must be defined)
  if( children.begin()==children.end() )
    throw ExceptionNoAppendersDefined(SYSTEM_SAVE_LOCATION);

  // iterate through all appenders
  for(Node::TNodesList::const_iterator it=children.begin();
      it!=children.end(); ++it)
  {
    const string &type=it->getName();
    const string &name=it->getAttributesList().getAttribute("name").getValue();

    // gather all options
    LoggerAppenderConfig::Options opts;
    const Node::TNodesList &appOpts=it->getChildrenList();
    for(Node::TNodesList::const_iterator itOpts=appOpts.begin();
        itOpts!=appOpts.end(); ++itOpts)
    {
      const string &name =itOpts->getName();
      const string &value=itOpts->getValuesString();
      opts[name].push_back(value);
    }

    // add appender with configuration to options
    apps.push_back( LoggerAppenderConfig(type, name, opts) );
  }

  return LoggerAppenders(apps);
}


} // namespace ConfigIO
