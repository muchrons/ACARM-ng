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

Reader::Reader(void):
  preludeLogger_("input.prelude.reader.preludelog")
{
}

Reader::DataPtr Reader::read(const unsigned int /*timeout*/)
{
  // TODO: implement
  DataPtr tmp;
  assert(tmp.get()==NULL);
  return tmp;
}

} // namespace Prelude
} // namespace Input
