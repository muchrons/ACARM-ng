/*
 * setThreadName.hpp
 *
 */
#ifndef INCLUDE_COMMONS_SETTHREADNAME_HPP_FILE
#define INCLUDE_COMMONS_SETTHREADNAME_HPP_FILE

/* public header */

#include <string>

namespace Commons
{
/** \brief set thread name using .
 *  \param name name for thread.
 */
void setThreadName(const std::string &threadName);
} // namespace Commons

#endif
