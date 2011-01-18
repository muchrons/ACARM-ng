/*
 * TestHelpers.t.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_MATH_TESTHTELPERS_T_HPP
#define INCLUDE_SYSTEM_MATH_TESTHTELPERS_T_HPP

#include <tut/tut.hpp>
#include <cmath>

namespace
{

void ensure_range(const char *msg, const double res, const double expected, const double prec)
{
  if( std::abs(res-expected)>prec )
    tut::ensure_equals(msg, res, expected);
} // ensure_range()

} // unnamed namespace

#endif
