/*
 * Parser.cpp
 *
 */
#include <cassert>

#include "XML/XMLpp/SaxParser.hpp"
#include "ConfigIO/Parser.hpp"
#include "ConfigIO/FileReader.hpp"
#include "ConfigIO/LoggerConfig.hpp"
#include "ConfigIO/PersistencyConfig.hpp"
#include "ConfigIO/ParseLoggerNodes.hpp"
#include "ConfigIO/ParseLoggerAppenders.hpp"
#include "ConfigIO/ParsePersistency.hpp"

using namespace std;
using namespace XML;


namespace ConfigIO
{

class Parser::ParserImpl
{
public:
  ParserImpl(const string &path):
    tree_( getParsedTree(path) ),
    parseNodes_(       tree_.getRoot().getChild("logger").getChild("nodes") ),
    parseAppenders_(   tree_.getRoot().getChild("logger").getChild("appenders") ),
    parsePersistency_( tree_.getRoot().getChild("persistency") ),
    loggerCfg_( parseNodes_.getDefaultNodeConfig(),
                parseNodes_.getNodes(),
                parseAppenders_.getAppenders() )
  {
  }

  const LoggerConfig &getLoggerConfig(void) const
  {
    return loggerCfg_;
  }

  const PersistencyConfig &getPersistencyConfig(void) const
  {
    return parsePersistency_.getConfig();
  }

private:
  inline Tree getParsedTree(const string &path) const
  {
    FileReader       reader(path);
    XMLpp::SaxParser sax;
    return sax.parse( reader.getString() );
  }

  Tree                 tree_;
  ParseLoggerNodes     parseNodes_;
  ParseLoggerAppenders parseAppenders_;
  ParsePersistency     parsePersistency_;

  LoggerConfig         loggerCfg_;
}; // class Parser::ParserImpl




//
// parser implementation
//

Parser::Parser(const std::string &path):
  pimpl_( new ParserImpl(path) )
{
  assert( pimpl_.get()!=NULL );
}

Parser::~Parser(void)
{
  assert( pimpl_.get()!=NULL );
  // this d-tor is required - ot forces compiler to generate its code here,
  // where ~ParserImpl is known. this ensures destruction will be called
  // as it should, with all d-tors.
}

const LoggerConfig &Parser::getLoggerConfig(void) const
{
  assert( pimpl_.get()!=NULL );
  return pimpl_->getLoggerConfig();
}

const PersistencyConfig &Parser::getPersistencyConfig(void) const
{
  assert( pimpl_.get()!=NULL );
  return pimpl_->getPersistencyConfig();
}

} // namespace ConfigIO
