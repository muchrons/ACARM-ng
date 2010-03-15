/*
 * ParseInputs.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_PARSEINPUTS_HPP_FILE
#define INCLUDE_CONFIGIO_PARSEINPUTS_HPP_FILE

#include "XML/Node.hpp"
#include "ConfigIO/InputConfig.hpp"
#include "ConfigIO/InputsConfigCollection.hpp"


namespace ConfigIO
{

/** \brief parser for inputs' configuration.
 */
class ParseInputs
{
public:
  /** \brief parses configuration and saves it internally.
   *  \param node node to start parsing from.
   */
  explicit ParseInputs(const XML::Node &node);

  /** \brief gets input's configuration.
   *  \return collection of inputs' configuration.
   */
  const InputsConfigCollection &getConfig(void) const
  {
    return icc_;
  }

private:
  void parse(const XML::Node &node);

  InputsConfigCollection icc_;
}; // class ParaseInputs

} // namespace ConfigIO

#endif
