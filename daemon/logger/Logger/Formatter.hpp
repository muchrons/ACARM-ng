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


namespace Logger
{

/** \brief class representing functionality of log messages formatting.
 */
class Formatter
{
public:
  /** \brief method for formatting messages.
   *  \param ssOut output stream to write to.
   *  \param ts    time stamp value.
   *  \param nn    node name of a logger.
   *  \param pri   priority message was logged with.
   *  \param file  name of the file where log has been reported.
   *  \param call  name of a funciton/method that caused log.
   *  \param line  line number in file, message was logged in.
   *  \param msg   message to be logged.
   */
  void format(std::stringstream &ssOut,
              const timeb       &ts,
              const NodeName    &nn,
              Priority           pri,
              const char        *file,
              const char        *call,
              unsigned int       line,
              const char        *msg) const;

  /** \brief swaps contents of two instances.
   *  \param other object to swap content with.
   */
  void swap(Formatter &other);

private:
  const char *pri2str(Priority pri) const;
  const char *strFix(const char *str) const;
  void appendValidMessage(std::stringstream &ssOut, const char *msg) const;
}; // class Formatter

} // namespace Logger

#endif
