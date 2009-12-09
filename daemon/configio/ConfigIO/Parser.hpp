/*
 * Parser.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_PARSER_HPP_FILE
#define INCLUDE_CONFIGIO_PARSER_HPP_FILE

/* public header */

#include <string>
#include <boost/scoped_ptr.hpp>

#include "System/Exception.hpp" // may be thrown by PIMPLed implementation
#include "ConfigIO/LoggerConfig.hpp"
#include "ConfigIO/PersistencyConfig.hpp"


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
  explicit Parser(const std::string &path="acarm_ng_config.xml");

  /** \brief deallocates internal data.
   */
  ~Parser(void);

  /** \brief gets logger's configuration, read from file.
   *  \return logger's configuration.
   */
  const LoggerConfig &getLoggerConfig(void) const;

  /** \brief gets persistency configuration, read from file.
   *  \return persistency configuration.
   */
  const PersistencyConfig &getPersistencyConfig(void) const;

private:
  // forward declaraion
  class ParserImpl;

  boost::scoped_ptr<ParserImpl> pimpl_;
}; // class Parser

} // namespace ConfigIO

#endif
