/*
 * Parse.hpp
 *
 * Note: this code is a merge of Parse.hpp and NamedParse.hpp (now removed).
 *       in case more general solution would be required, it must be split back
 *       into two classes and common code separeted and reused, possibly via
 *       CRTP callbacks.
 */
#ifndef INCLUDE_CONFIGIO_GENERIC_PARSE_HPP_FILE
#define INCLUDE_CONFIGIO_GENERIC_PARSE_HPP_FILE

#include "XML/Node.hpp"
#include "ConfigIO/Preprocessor/Parse.hpp"
#include "ConfigIO/ExceptionParseError.hpp"

namespace ConfigIO
{
namespace Generic
{
namespace detail
{
/** \brief meta-program for building configuration with or w-out preprocessor's config.
 *
 *  gnereic template builds with preprocessor support.
 */
template<typename TConfig, bool withPreproc>
struct CfgBuilder
{
  /** \brief create confiuration w-out preprocessor.
   *  \param type name of the type to be created.
   *  \param opts configuration options.
   *  \return created configuration.
   */
  static TConfig build(const typename TConfig::TypeName &type,
                       const typename TConfig::Options  &opts,
                       const Preprocessor::Config       * /*ppCfg*/)
  {
    return TConfig(type, opts);
  }
};
/** \brief meta-program for building configuration with or w-out preprocessor's config.
 *
 *  template specialization builds without preprocessor support.
 */
template<typename TConfig>
struct CfgBuilder<TConfig, true>
{
  /** \brief create confiuration w-out preprocessor.
   *  \param type  name of the type to be created.
   *  \param opts  configuration options.
   *  \param ppCfg preprocessor configuration (can be NULL).
   *  \return created configuration.
   */
  static TConfig build(const typename TConfig::TypeName &type,
                       const typename TConfig::Options  &opts,
                       const Preprocessor::Config       *ppCfg)
  {
    if(ppCfg!=NULL)
      return TConfig(type, opts, *ppCfg);
    return TConfig(type, opts);
  }
};
} // namespace detail

/** \brief parser for inputs' configuration.
 */
template<typename TConfig, typename TConfigCollection, bool usePreproc>
class Parse
{
public:
  /** \brief parses configuration and saves it internally.
   *  \param node node to start parsing from.
   */
  explicit Parse(const XML::Node &node)
  {
    parseNamed(node);
  }

  /** \brief gets configuration collection.
   *  \return collection of configurations.
   */
  const TConfigCollection &getConfig(void) const
  {
    return cc_;
  }

private:
  void parseNamed(const XML::Node &node)
  {
    // iterate through all of the definitions
    const XML::Node::TNodesList &elements=node.getChildrenList();
    for(XML::Node::TNodesList::const_iterator eit=elements.begin(); eit!=elements.end(); ++eit)
    {
      const typename TConfig::TypeName &type=eit->getName();
      const typename TConfig::TypeName &name=eit->getAttributesList().getAttribute("name").getValue();

      if(isNameUnique(elements, name) != true)
        throw ExceptionParseError(SYSTEM_SAVE_LOCATION, "name: " + name + " is not unique");

      typename TConfig::Options   options;
      Preprocessor::Config        ppCfg;
      const Preprocessor::Config *ppCfgPtr=NULL;
      // get all options to a single collection
      const XML::Node::TNodesList &children=eit->getChildrenList();
      for(XML::Node::TNodesList::const_iterator it=children.begin(); it!=children.end(); ++it)
      {
        if(usePreproc && it->getName()=="preprocessor")
        {
          const Preprocessor::Parse p(*it);
          ppCfg   =p.getConfig();
          ppCfgPtr=&ppCfg;
        }
        else
          options[ it->getName() ] = it->getValuesString();
      }

      options["name"]=name;
      // add new entry
      cc_.push_back( detail::CfgBuilder<TConfig, usePreproc>::build(type, options, ppCfgPtr) );
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
        // NOTE: name is not unigue, when there are at least two the same names
        if(++cnt > 1)
          return false;
    }
    return true;
  }

  TConfigCollection cc_;
}; // class Parase

} // namespace Generic
} // namespace ConfigIO

#endif
