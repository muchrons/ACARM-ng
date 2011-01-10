/*
 * Backtrace.hpp
 */
#ifndef INCLUDE_SYSTEM_BACKTRACE_HPP_FILE
#define INCLUDE_SYSTEM_BACKTRACE_HPP_FILE

/* public header */

#include <vector>
#include <string>
#include <stdexcept>

namespace System
{
/** \brief saves current backtrace.
 *  uses GNU-extension to read backtrace pieces of information. using this it
 *  is possible to use backtrace dumps for logs.
 *  \note in order for this class to work properly code must be compiled with
 *        -rdynamic switch.
 */
class Backtrace
{
private:
  typedef std::vector<std::string> BacktraceLog;
public:
  /** \brief declaration of bidirectiona iterator for stack.
   */
  typedef BacktraceLog::const_iterator const_iterator;
  /** \brief creates call stack dump and saves it.
   */
  Backtrace(void);
  /** \brief returns iterator to the top-most element on the stack.
   *  \return begin iterator.
   */
  const_iterator begin(void) const
  {
    return bt_.begin();
  }
  /** \brief returns end-indicating iterator.
   *  \return end iterator.
   */
  const_iterator end(void) const
  {
    return bt_.end();
  }
  /** \brief gets number of entries held inside.
   *  \return number of entries on the stack.
   */
  size_t size(void) const
  {
    return bt_.size();
  }
  /** \brief converts whole stack to string.
   *  \note this is usefull for printing out output.
   *  \return string with whole stack trace info.
   */
  std::string toString(void) const;

private:
  BacktraceLog bt_;
}; // class Backtrace

} // namespace System

#endif
