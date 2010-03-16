/*
 * ExceptionUnexpectedNULL.cpp
 *
 */
#include <string>

#include "System/Backtrace.hpp"
#include "Commons/ExceptionUnexpectedNULL.hpp"

using namespace std;


namespace Commons
{

ExceptionUnexpectedNULL::ExceptionUnexpectedNULL(const Location &where):
  Exception(where,
            string("unexpected NULL pointer received; stactrace is: ") +
              System::Backtrace().toString() )  // log call-stack as well.
{
}

} // namespace Commons
