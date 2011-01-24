/*
 * InputConfig.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_INPUTCONFIG_HPP_FILE
#define INCLUDE_CONFIGIO_INPUTCONFIG_HPP_FILE

/* public header */

#include <vector>

#include "ConfigIO/Generic/Config.hpp"

namespace ConfigIO
{

/** \brief class representing configuration of input.
 */
class InputConfig: public Generic::Config<InputConfig>
{
public:
  /** \brief creates input's configuration.
   *  \param type    input's type.
   *  \param options options for given input.
   */
  InputConfig(const TypeName &type,
              const Options  &options):
    Generic::Config<InputConfig>(type, options)
  {
  }

  /** \brief gets input's name.
   *  \return inputs's name.
   */
  std::string getInputName(void)
  {
    return get("name");
  }
}; // class InputConfig

} // namespace ConfigIO

#endif
