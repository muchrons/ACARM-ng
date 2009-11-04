/*
 * LoggerNodes.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_LOGGERNODES_HPP_FILE
#define INCLUDE_CONFIGIO_LOGGERNODES_HPP_FILE

/* public header */

#include <vector>
#include <string>

#include "ConfigIO/LoggerNodeConfig.hpp"


namespace ConfigIO
{

/** \brief exception informing about duplicated entries
 */
struct ExceptionDuplicatedNodeName: public Exception
{
  /** \brief create exception of invalid (duplicated) node name
   *  \param where place where problem has been detected.
   *  \param name  name that is duplicated.
   */
  ExceptionDuplicatedNodeName(const char        *where,
                              const std::string &name):
    Exception( std::string( ensureValidString(where) ) +
               ": duplicated node name: " + name )
  {
  }
}; // struct ExceptionDuplicatedNodeName


/** \brief collection of nodes configurations.
 */
class LoggerNodes
{
public:
  /** \brief nodes' configuration collection.
   */
  typedef std::vector<LoggerNodeConfig> Nodes;

  /** \brief const iterator declaration.
   */
  typedef Nodes::const_iterator const_iterator;

  /** \brief creates set of logger nodes' configuration.
   *  \param ns collection of nodes' configuration.
   *  \note if entries in ns will not be unique, c-tor will throw.
   */
  explicit LoggerNodes(const Nodes &ns);

  /** \brief gets const iterator to collection's begin.
   *  \return beign const iterator.
   */
  const_iterator begin(void) const
  {
    return ns_.begin();
  }

  /** \brief gets const iterator to collection's end.
   *  \return end const iterator.
   */
  const_iterator end(void) const
  {
    return ns_.end();
  }

private:
  Nodes ns_;
}; // class LoggerNodes

} // namespace ConfigIO

#endif
