/*
 * NamedParse.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_GENERIC_NAMEDPARSE_HPP_FILE
#define INCLUDE_CONFIGIO_GENERIC_NAMEDPARSE_HPP_FILE

#include "XML/Node.hpp"
#include "ConfigIO/ExceptionParseError.hpp"

// TODO: reorganize code to use Parse class

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
      const typename TConfig::TypeName &name=eit->getAttributesList().getAttribute("name").getValue();
      if(isNameUnique(elements, name) != true)
        throw ExceptionParseError(SYSTEM_SAVE_LOCATION, "name: " + name + " is not unique");
      typename TConfig::Options         options;
      // get all options to a single collection
      const XML::Node::TNodesList &children=eit->getChildrenList();
      for(XML::Node::TNodesList::const_iterator it=children.begin();
          it!=children.end(); ++it)
        options[ it->getName() ] = it->getValuesString();
      options[ "name" ] = name;
      // add new entry
      cc_.push_back( TConfig(type, options) );
    }
  }

  bool isNameUnique(const XML::Node::TNodesList &elements, const std::string &name)
  {
    unsigned int cnt = 0;
    for(XML::Node::TNodesList::const_iterator eit=elements.begin();
        eit!=elements.end(); ++eit)
    {
      const std::string tmpName = eit->getAttributesList().getAttribute("name").getValue();
      if(name == tmpName)
        // TODO: why not simply 'return false'?
        cnt++;
    }
    if(cnt > 1)
      return false;
    return true;
  }

  TConfigCollection cc_;
}; // class Parase

} // namespace Generic
} // namespace ConfigIO

#endif
