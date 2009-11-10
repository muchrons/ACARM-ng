/*
 * ReaderBuilder.cpp
 *
 */

#include "Input/Prelude/ReaderBuilder.hpp"
#include "Input/Prelude/Reader.hpp"

// TODO: add automatic registration to the factory

namespace Input
{
namespace Prelude
{

ReaderBuilder::ReaderBuilder(void):
  name_("prelude")
{
}

ReaderBuilder::FactoryPtr ReaderBuilder::build(const Options &/*options*/) const
{
  // TODO: implement this
  return ReaderBuilder::FactoryPtr( new Reader() );
}

const ReaderBuilder::FactoryTypeName &ReaderBuilder::getTypeName(void) const
{
  return name_;
}

} // namespace Prelude
} // namespace Input
