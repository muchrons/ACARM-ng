/*
 * Parse.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_GENERIC_PARSE_HPP_FILE
#define INCLUDE_CONFIGIO_GENERIC_PARSE_HPP_FILE

#include "XML/Node.hpp"
#include "ConfigIO/ExceptionParseError.hpp"

namespace ConfigIO
{
namespace Generic
{

/** \brief parser for inputs' configuration.
 */
template<typename TConfig, typename TConfigCollection>
class Parse
{
public:
  /** \brief parses configuration and saves it internally.
   *  \param node node to start parsing from.
   */
  explicit Parse(const XML::Node &node)
  {
    parse(node);
  }

  /** \brief gets configuration collection.
   *  \return collection of configurations.
   */
  const TConfigCollection &getConfig(void) const
  {
    return cc_;
  }

private:
  void parse(const XML::Node &node)
  {
    // iterate through all of the definitions
    const XML::Node::TNodesList &elements=node.getChildrenList();
    for(XML::Node::TNodesList::const_iterator eit=elements.begin();
        eit!=elements.end(); ++eit)
    {
      const typename TConfig::TypeName &type=eit->getName();
      // TODO: do not keep dead code here - use version control system instead.
      //const typename TConfig::TypeName &name=eit->getAttributesList().getAttribute("name").getValue();
      typename TConfig::Options         options;

      // get all options to a single collection
      const XML::Node::TNodesList &children=eit->getChildrenList();
      for(XML::Node::TNodesList::const_iterator it=children.begin();
          it!=children.end(); ++it)
        options[ it->getName() ] = it->getValuesString();
      // TODO: do not keep dead code here - use version control system instead.
      //options[ "name" ] = name;
      // add new entry
      cc_.push_back( TConfig(type, options) );
    }
  }

  TConfigCollection cc_;
}; // class Parase

} // namespace Generic
} // namespace ConfigIO

#endif
