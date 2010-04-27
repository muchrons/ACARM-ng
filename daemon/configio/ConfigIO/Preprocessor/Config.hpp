/*
 * Config.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_PREPROCESSOR_CONFIG_HPP_FILE
#define INCLUDE_CONFIGIO_PREPROCESSOR_CONFIG_HPP_FILE

/* public header */

#include <vector>

#include "ConfigIO/Preprocessor/Section.hpp"

namespace ConfigIO
{
namespace Preprocessor
{

/** \brief class representing preprocessor implementation.
 */
class Config
{
public:
  /** \brief type representing collection of sections. */
  typedef std::vector<Section> Sections;

  /** \brief create configuration object.
   *  \param sections sections present in configuration.
   */
  explicit Config(const Sections &sections):
    sections_(sections)
  {
  }

  /** \brief gets sections objects.
   *  \return sections of the configration.
   */
  const Sections &getSections(void) const
  {
    return sections_;
  }

private:
  Sections sections_;
}; // class Config

} // namespace Preprocessor
} // namespace ConfigIO

#endif
