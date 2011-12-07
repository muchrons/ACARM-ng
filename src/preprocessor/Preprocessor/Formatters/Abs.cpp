/*
 * Abs.cpp
 *
 */
#include <cmath>
#include <cassert>

#include "Commons/NumberWrapper.hpp"
#include "Preprocessor/Formatters/Abs.hpp"


namespace Preprocessor
{
namespace Formatters
{

Abs::Abs(BasePtrNN arg):
  Base("absolute", Formatters(1, arg) )
{
}

std::string Abs::execImpl(const Arguments &args) const
{
  assert( args.size()==1 );
  Commons::NumberWrapper n(args[0]);
  //if(
  return "aaaaaaaa";    // TODO
}

} // namespace Formatters
} // namespace Preprocessor
