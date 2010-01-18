/*
 * Parser.cpp
 *
 */
#include <cassert>

#include "XML/XMLpp/SaxParser.hpp"
#include "ConfigIO/Parser.hpp"
#include "ConfigIO/FileReader.hpp"

using namespace std;
using namespace XML;

namespace ConfigIO
{

namespace
{
/** \brief gets XML tree after parsing a given file.
 *  \param path path to file to be parsed.
 *  \return parsed XML tree.
 */
inline Tree getParsedTree(const string &path)
{
  FileReader       reader(path);
  XMLpp::SaxParser sax;
  return sax.parse( reader.getString() );
} // getParsedTree()
} // unnamed namespace

Parser::Parser(const std::string &path):
    tree_( getParsedTree(path) ),
    parseNodes_(       tree_.getRoot().getChild("logger").getChild("nodes") ),
    parseAppenders_(   tree_.getRoot().getChild("logger").getChild("appenders") ),
    parsePersistency_( tree_.getRoot().getChild("persistency") ),
    loggerCfg_( parseNodes_.getDefaultNodeConfig(),
                parseNodes_.getNodes(),
                parseAppenders_.getAppenders() )
{
}

const LoggerConfig &Parser::getLoggerConfig(void) const
{
  return loggerCfg_;
}

const PersistencyConfig &Parser::getPersistencyConfig(void) const
{
  return parsePersistency_.getConfig();
}

} // namespace ConfigIO
