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
inline Tree getParsedTree(const boost::filesystem::path &path)
{
  FileReader       reader(path);
  XMLpp::SaxParser sax;
  return sax.parseContent( reader.getString() );
} // getParsedTree()
} // unnamed namespace

Parser::Parser(const boost::filesystem::path &path):
    tree_( getParsedTree(path) ),
    parseGeneralConfig_( tree_.getRoot().getChild("general") ),
    parsePersistency_(   tree_.getRoot().getChild("persistency") ),
    parseFilters_(       tree_.getRoot().getChild("filters") ),
    parseTriggers_(      tree_.getRoot().getChild("triggers") ),
    parseInputs_(        tree_.getRoot().getChild("inputs") ),
    parseNodes_(         tree_.getRoot().getChild("logger").getChild("nodes") ),
    parseAppenders_(     tree_.getRoot().getChild("logger").getChild("appenders") ),
    loggerCfg_( parseNodes_.getDefaultNodeConfig(),
                parseNodes_.getNodes(),
                parseAppenders_.getAppenders() ),
    parsePreprocessor_(  tree_.getRoot().getChild("preprocessor") )
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

const FiltersConfigCollection &Parser::getFiltersConfig(void) const
{
  return parseFilters_.getConfig();
}

const TriggersConfigCollection &Parser::getTriggersConfig(void) const
{
  return parseTriggers_.getConfig();
}

const InputsConfigCollection &Parser::getInputsConfig(void) const
{
  return parseInputs_.getConfig();
}

const Preprocessor::Config &Parser::getPreprocessorConfig(void) const
{
  return parsePreprocessor_.getConfig();
}

const GeneralConfig &Parser::getGeneralConfig(void) const
{
  return parseGeneralConfig_.getConfig();
}

} // namespace ConfigIO
