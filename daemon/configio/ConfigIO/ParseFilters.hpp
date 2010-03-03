/*
 * ParseFilters.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_PARSEFILTERS_HPP_FILE
#define INCLUDE_CONFIGIO_PARSEFILTERS_HPP_FILE

#include <vector>

#include "XML/Node.hpp"
#include "ConfigIO/FilterConfig.hpp"


namespace ConfigIO
{

/** \brief parser for filters' configuration.
 */
class ParseFilters
{
public:
  /** \brief collection of configurations for given filters. */
  typedef std::vector<FilterConfig> FiltersConfigCollection;

  /** \brief parses configuration and saves it internally.
   *  \param node node to start parsing from.
   */
  explicit ParseFilters(const XML::Node &node);

  /** \brief gets filter's configuration.
   *  \return collection of filters' configuration.
   */
  const FiltersConfigCollection &getConfig(void) const
  {
    return fcc_;
  }

private:
  void parse(const XML::Node &node);

  FiltersConfigCollection fcc_;
}; // class ParaseFilters

} // namespace ConfigIO

#endif
