/*
 * Parser.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_PARSER_HPP_FILE
#define INCLUDE_CONFIGIO_PARSER_HPP_FILE

/* public header */

#include <string>
#include <boost/scoped_ptr.hpp>

#include "ConfigIO/LoggerConfig.hpp"
#include "ConfigIO/PersistencyConfig.hpp"

// TODO: implement
// TODO: test
// TODO: comment

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
  explicit Parser(const std::string &path);

  ~Parser(void);

  const LoggerConfig &getLoggerConfig(void) const;

  const PersistencyConfig &getPersistencyConfig(void) const;

private:
  // forward declaraion
  class ParserImpl;

  boost::scoped_ptr<ParserImpl> pimpl_;
}; // class Parser

} // namespace ConfigIO

#endif
