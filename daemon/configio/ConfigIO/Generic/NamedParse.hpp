/*
 * NamedParse.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_GENERIC_NAMEDPARSE_HPP_FILE
#define INCLUDE_CONFIGIO_GENERIC_NAMEDPARSE_HPP_FILE

#include "XML/Node.hpp"

namespace ConfigIO
{
namespace Generic
{

/** \brief parser for inputs' configuration.
 */
template<typename TConfig, typename TConfigCollection>
class NamedParse
{
public:
  /** \brief parses configuration and saves it internally.
   *  \param node node to start parsing from.
   */
  explicit NamedParse(const XML::Node &node)
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
      typename TConfig::Options         options;

      // get all options to a single collection
      const XML::Node::TNodesList &children=eit->getChildrenList();
      for(XML::Node::TNodesList::const_iterator it=children.begin();
          it!=children.end(); ++it)
        options[ it->getName() ] = it->getValuesString();

      // add new entry
      cc_.push_back( TConfig::create(*eit, type, options) );
      //cc_.push_back( TConfig(type, name, options) );
    }
  }
  TConfigCollection cc_;
}; // class NamedParase

} // namespace Generic
} // namespace ConfigIO

#endif
