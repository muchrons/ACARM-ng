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

/** \brief exception informing about invalid port number.
 */
struct ExceptionPortNumberConversionFailed: public Exception
{
  /** \brief create exception of no appenders defined
   *  \param where place where problem has been detected.
   *  \param port  port string that was not valid.
   */
  ExceptionPortNumberConversionFailed(const char        *where,
                                      const std::string  port):
    Exception( std::string( ensureValidString(where) ) +
               ": invalid port in config: " + port )
  {
  }
}; // struct ExceptionPortNumberConversionFailed


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

  PersistencyConfig::PortNumber parsePort(const std::string &port) const;

  PersistencyConfig cfg_;
}; // class ParasePersistency

} // namespace ConfigIO

#endif
