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
  explicit Parser(const std::string &str);

  unsigned int get(void) const
  {
    return num_;
  }

private:
  const unsigned int num_;
}; // class Parser

} // namespace LogSplitter

#endif
