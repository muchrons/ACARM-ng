/*
 * Parser.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_PARSER_HPP_FILE
#define INCLUDE_CONFIGIO_PARSER_HPP_FILE

/* public header */

#include <string>


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

  // TODO
}; // class Parser

} // namespace ConfigIO

#endif
