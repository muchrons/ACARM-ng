/*
 * ReaderBuilder.hpp
 *
 */
#ifndef INCLUDE_INPUT_FILE_READERBUILDER_HPP_FILE
#define INCLUDE_INPUT_FILE_READERBUILDER_HPP_FILE

#include <string>

#include "Logger/Node.hpp"
#include "Input/Factory.hpp"
#include "Input/File/Exception.hpp"

namespace Input
{
namespace File
{

/** \brief builder for reader of prelude format.
 */
class ReaderBuilder: public Factory::TFactoryBuilderBase
{
public:
  /** \brief creates builder of a readers of type Input::File.
   */
  ReaderBuilder(void);

private:
  virtual FactoryPtr buildImpl(const Options &options) const;
  virtual const FactoryTypeName &getTypeNameImpl(void) const;

  const Core::Types::Proc::TypeName type_;
  const Logger::Node                log_;
}; // class ReaderBuilder

} // namespace File
} // namespace Input

#endif
