/*
 * Formatter.hpp
 *
 */
#ifndef INCLUDE_LOGGER_FORMATTER_HPP_FILE
#define INCLUDE_LOGGER_FORMATTER_HPP_FILE

#include <sstream>
#include <sys/timeb.h>

#include "Logger/NodeName.hpp"
#include "Logger/Priority.hpp"

// TODO: comment
// TODO: test

namespace Logger
{

class Formatter
{
public:
  void format(std::stringstream &ssOut,
              const timeb       &ts,
              const NodeName    &nn,
              Priority           pri,
              const char        *file,
              const char        *call,
              unsigned int       line,
              const char        *msg) const;

private:
  const char *pri2str(Priority pri) const;
  const char *strFix(const char *str) const;
}; // class Formatter

} // namespace Logger

#endif
