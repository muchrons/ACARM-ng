/*
 * ParseFilters.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_PARSEFILTERS_HPP_FILE
#define INCLUDE_CONFIGIO_PARSEFILTERS_HPP_FILE

#include "XML/Node.hpp"
#include "ConfigIO/FilterConfig.hpp"
#include "ConfigIO/FiltersConfigCollection.hpp"
#include "ConfigIO/FiltersConfigCollection.hpp"
#include "ConfigIO/Generic/NamedParse.hpp"


namespace ConfigIO
{

/** \brief parser for filters' configuration.
 */
class ParseFilters: public Generic::NamedParse<FilterConfig, FiltersConfigCollection>
{
public:
  /** \brief parses configuration and saves it internally.
   *  \param node node to start parsing from.
   */
  explicit ParseFilters(const XML::Node &node);
}; // class ParaseFilters

} // namespace ConfigIO

#endif
