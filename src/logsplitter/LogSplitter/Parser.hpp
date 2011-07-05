/*
 * Parser.hpp
 *
 */
#ifndef INCLUDE_LOGSPLITTER_PARSER_HPP_FILE
#define INCLUDE_LOGSPLITTER_PARSER_HPP_FILE

#include <string>
#include <stdexcept>

#include <boost/noncopyable.hpp>

namespace LogSplitter
{

/** \brief parser looking for " [<digits>] " sequence.
 */
class Parser: private boost::noncopyable
{
public:
  /** \brief parse given string, looking for a thread number.
   *  \param str string to be parsed.
   */
  explicit Parser(const std::string &str);

  /** \brief get parsed line number.
   *  \return line number parsed from string.
   */
  unsigned int get(void) const
  {
    return num_;
  }

private:
  const unsigned int num_;
}; // class Parser

} // namespace LogSplitter

#endif
