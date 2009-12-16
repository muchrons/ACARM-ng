/*
 * TestHelpers.t.hpp
 *
 */
#ifndef INCLUDE_LOGGER_TESTHELPERS_T_HPP_FILE
#define INCLUDE_LOGGER_TESTHELPERS_T_HPP_FILE

#include <string>

namespace Logger
{

// returns last logged message.
std::string getLastLogMessage(void);

// ensures that last logged message has 'part' string inside. if not
// tut::fail() is called with proper message.
void ensureLoggedPart(const char *part);

} // namespace Logger

#endif
