/*
 * Reader.cpp
 *
 */
#include <cassert>

#include "Input/Prelude/Reader.hpp"

namespace Input
{
namespace Prelude
{

Reader::DataPtr Reader::read(const unsigned int /*timeout*/)
{
  // TODO: implement
  DataPtr tmp;
  assert(tmp.get()==NULL);
  return tmp;
}

} // namespace Prelude
} // namespace Input
