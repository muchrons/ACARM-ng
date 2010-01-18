/*
 * ParsePersistency.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_PARSEPERSISTENCY_HPP_FILE
#define INCLUDE_CONFIGIO_PARSEPERSISTENCY_HPP_FILE

#include <string>

#include "XML/Node.hpp"
#include "ConfigIO/PersistencyConfig.hpp"


namespace ConfigIO
{

/** \brief parser for persistency configuration.
 */
class ParsePersistency
{
public:
  /** \brief parses persistency configuration and saves it internally.
   *  \param node node to start parsing from.
   */
  explicit ParsePersistency(const XML::Node &node);

  /** \brief gets persistency configuration.
   *  \return persistency configuration.
   */
  const PersistencyConfig &getConfig(void) const
  {
    return cfg_;
  }

private:
  PersistencyConfig parse(const XML::Node &node) const;

  PersistencyConfig cfg_;
}; // class ParasePersistency

} // namespace ConfigIO

#endif
