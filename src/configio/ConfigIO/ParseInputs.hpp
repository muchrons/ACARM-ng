/*
 * ParseInputs.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_PARSEINPUTS_HPP_FILE
#define INCLUDE_CONFIGIO_PARSEINPUTS_HPP_FILE

#include "XML/Node.hpp"
#include "ConfigIO/InputConfig.hpp"
#include "ConfigIO/InputsConfigCollection.hpp"
#include "ConfigIO/Generic/Parse.hpp"


namespace ConfigIO
{

/** \brief parser for inputs' configuration.
 */
class ParseInputs: public Generic::Parse<InputConfig, InputsConfigCollection, false>
{
public:
  /** \brief parses configuration and saves it internally.
   *  \param node node to start parsing from.
   */
  explicit ParseInputs(const XML::Node &node);
}; // class ParaseInputs

} // namespace ConfigIO

#endif
