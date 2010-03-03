/*
 * Parser.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_PARSER_HPP_FILE
#define INCLUDE_CONFIGIO_PARSER_HPP_FILE

#include <string>
#include <boost/scoped_ptr.hpp>

#include "System/Exception.hpp" // may be thrown by implementation
#include "XML/Tree.hpp"
#include "ConfigIO/LoggerConfig.hpp"
#include "ConfigIO/PersistencyConfig.hpp"
#include "ConfigIO/ParseLoggerNodes.hpp"
#include "ConfigIO/ParseLoggerAppenders.hpp"
#include "ConfigIO/ParsePersistency.hpp"
#include "ConfigIO/ParseFilters.hpp"


namespace ConfigIO
{

/** \brief config file parser.
 */
class Parser
{
public:
  /** \brief creates parset for a given file.
   *  \param path file to be parsed.
   */
  explicit Parser(const std::string &path=DEFAULT_CONFIGIO_CONFIG_FILE);

  /** \brief gets logger's configuration, read from file.
   *  \return logger's configuration.
   */
  const LoggerConfig &getLoggerConfig(void) const;

  /** \brief gets persistency configuration, read from file.
   *  \return persistency configuration.
   */
  const PersistencyConfig &getPersistencyConfig(void) const;

  /** \brief gets filters' configuration, read from file.
   *  \return filters' configuration.
   */
  const ParseFilters::FiltersConfigCollection &getFiltersConfig(void) const;

private:
  XML::Tree            tree_;
  ParseLoggerNodes     parseNodes_;
  ParseLoggerAppenders parseAppenders_;
  ParsePersistency     parsePersistency_;
  ParseFilters         parseFilters_;

  LoggerConfig         loggerCfg_;
}; // class Parser

} // namespace ConfigIO

#endif
