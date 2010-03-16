/*
 * ReaderBuilder.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_READERBUILDER_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_READERBUILDER_HPP_FILE

#include <string>

#include "Logger/Logger.hpp"
#include "Input/Factory.hpp"
#include "Input/Exception.hpp"

namespace Input
{
namespace Prelude
{

/** \brief builder for reader of prelude format.
 */
class ReaderBuilder: public Factory::TFactoryBuilderBase
{
public:
  /** \brief creates builder of a readers for Input::Prelude.
   */
  ReaderBuilder(void);

private:
  virtual FactoryPtr buildImpl(const Options &options) const;
  virtual const FactoryTypeName &getTypeNameImpl(void) const;

  const std::string  name_;
  const Logger::Node log_;
}; // class ReaderBuilder

} // namespace Prelude
} // namespace Input

#endif
