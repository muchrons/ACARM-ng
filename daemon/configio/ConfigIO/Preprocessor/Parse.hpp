/*
 * Parse.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_PREPROCESSOR_PARSE_HPP_FILE
#define INCLUDE_CONFIGIO_PREPROCESSOR_PARSE_HPP_FILE

#include "XML/Node.hpp"
#include "ConfigIO/ExceptionParseError.hpp"
#include "ConfigIO/Preprocessor/Config.hpp"

// TODO: test this code

namespace ConfigIO
{
namespace Preprocessor
{

/** \brief class for parsing preprocessor's config.
 */
class Parse
{
public:
  /** \brief parses configuration sub-tree.
   *  \param root root node for subtree to parse.
   */
  explicit Parse(const XML::Node &root);
  /** \brief gets parsed configuration.
   *  \return parsed configuration.
   */
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
