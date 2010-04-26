/*
 * Parse.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_PREPROCESSOR_PARSE_HPP_FILE
#define INCLUDE_CONFIGIO_PREPROCESSOR_PARSE_HPP_FILE

// TODO: comment this code

#include "XML/Node.hpp"
#include "ConfigIO/ExceptionParseError.hpp"
#include "ConfigIO/Preprocessor/Config.hpp"

namespace ConfigIO
{
namespace Preprocessor
{

/** \brief class for parsing preprocessor's config.
 */
class Parse
{
public:
  explicit Parse(const XML::Node &root);

  const Config &getConfig(void) const
  {
    return cfg_;
  }

private:
  Config cfg_;
}; // class Parse

} // namespace Preprocessor
} // namespace ConfigIO

#endif
