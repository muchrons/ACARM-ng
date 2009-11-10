/*
 * ReaderBuilder.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_READERBUILDER_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_READERBUILDER_HPP_FILE

/* public header */

#include <string>

#include "Input/ReadersFactory.hpp"

namespace Input
{
namespace Prelude
{

class ReaderBuilder: public ReadersFactory::TFactoryBuilderBase
{
public:
  /** \brief creates builder of a readers for Input::Prelude
   */
  ReaderBuilder(void);
  /** \brief builds required factory, with a given options.
   *  \param options options to be passed to the new instance.
   *  \return new factory instance.
   */
  virtual FactoryPtr build(const Options &options) const;
  /** \brief gets name of type it can build.
   *  \return name of factory it can build.
   */
  virtual const FactoryTypeName &getTypeName(void) const;

private:
  const std::string name_;
}; // class ReaderBuilder

} // namespace Prelude
} // namespace Input

#endif
