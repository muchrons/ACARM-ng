/*
 * FunctionName.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_FUNCTIONNAME_HPP_FILE
#define INCLUDE_SYSTEM_FUNCTIONNAME_HPP_FILE

/* public header */

// defines SYSTEM_FUNCTION_NAME macro that gets funciton name.
#if __GNUC__ >= 3
  #define SYSTEM_FUNCTION_NAME __PRETTY_FUNCTION__
#else
  #define SYSTEM_FUNCTION_NAME __FUNCTION__
#endif

#endif
