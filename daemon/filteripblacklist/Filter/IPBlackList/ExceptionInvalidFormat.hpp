/*
 * ExceptionInvalidFormat.hpp
 *
 */
#ifndef INCLUDE_FILTER_IPBLACKLIST_EXCEPTIONINVALIDFORMAT_HPP_FILE
#define INCLUDE_FILTER_IPBLACKLIST_EXCEPTIONINVALIDFORMAT_HPP_FILE

/* public header */

#include "Filter/Exception.hpp"

namespace Filter
{
namespace IPBlackList
{
/** \brief exception raised when input format is invalid
 */
class ExceptionInvalidFormat: public Filter::Exception
{
public:
  /** \brief create execption with given message.
   *  \param where  place where exception has been thrown.
   *  \param around place around where problem has been spotted.
   */
  ExceptionInvalidFormat(const Location &where, const char *around);

private:
  std::string makeAround(const char *in) const;
}; // class ExceptionInvalidFormat

} // namespace IPBlackList
} // namespace Filter

#endif
