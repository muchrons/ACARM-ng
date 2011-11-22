/*
 * ConfigFileReader.cpp
 *
 */
#include <vector>

#include "ConfigConsts/configio.hpp"
#include "XML/XMLpp/SaxParser.hpp"
#include "ConfigIO/ConfigFileReader.hpp"
#include "ConfigIO/FileReader.hpp"
#include "ConfigIO/Parser.hpp"

using namespace Base::Filesystem;


namespace ConfigIO
{

ConfigFileReader::ExceptionInclusionLoop::ExceptionInclusionLoop(const Location               &where,
                                                                 const Base::Filesystem::Path &reIncluded):
  Exception(where, cc("file inclusion loop detected when (re)including file '", reIncluded, "'") )
{
}

ConfigFileReader::ExceptionInvalidInclude::ExceptionInvalidInclude(const Location               &where,
                                                                   const Base::Filesystem::Path &included):
  Exception(where, cc("inclusion of invalid include file requested: '", included, "'") )
{
}


namespace
{

typedef std::vector<Path> IncludeChain;


Path absoluteDir(const Path &in)
{
  return parentPath( boost::filesystem::system_complete(in) );
}


XML::Tree readAndParse(const Path &path)
{
  FileReader            reader(path);
  XML::XMLpp::SaxParser sax;
  return sax.parseContent( reader.getString() );
} // readAndParse()


bool formsInclusionLoop(const IncludeChain &chain, const Path &newPath)
{
  for(IncludeChain::const_iterator it=chain.begin(); it!=chain.end(); ++it)
    if(*it==newPath)
      return true;
  return false;
} // formsInclusionLoop()


Path makePath(const Path &rootDir, const Path &name)
{
  if( isAbsolute(name) )
    return name;
  return rootDir/name;
} // makePath()


// TODO: this implementation has been done in a fast way, using available components.
//       it could be reimplemented using sax parsing though. this would remove
//       copying, that takes place all the time here.


void appendAndExpand(XML::Node &dst, const Path &path, const char *expectedRoot, IncludeChain chain);


void appendAndExpand(XML::Node &dst, const XML::Node &src, const Path &rootDir, const IncludeChain &chain)
{
  const char                  *include ="include";
  const XML::Node::TNodesList &children=src.getChildrenList();
  for(XML::Node::TNodesList::const_iterator it=children.begin(); it!=children.end(); ++it)
  {
    // expand include files "in place"
    if( it->getName()==include )
    {
      const Path incPath=makePath(rootDir, it->getValuesString());
      if( formsInclusionLoop(chain, incPath) )
        throw ConfigFileReader::ExceptionInclusionLoop(SYSTEM_SAVE_LOCATION, incPath);
      appendAndExpand(dst, incPath, include, chain);
      continue;
    }

    // add new node to the set
    XML::Node *childPtr=dst.addChild( XML::Node( it->getName() ) );
    if(childPtr==NULL)
      throw Exception(SYSTEM_SAVE_LOCATION, "addition of node failed");
    XML::Node &child=*childPtr;

    // copy all attributes
    const XML::AttributesList &attr=it->getAttributesList();
    for(XML::AttributesList::const_iterator ait=attr.begin(); ait!=attr.end(); ++ait)
      child.addAttribute(*ait);

    // copy all values
    const XML::Node::TValuesList &vals=it->getValuesList();
    for(XML::Node::TValuesList::const_iterator vit=vals.begin(); vit!=vals.end(); ++vit)
      child.addValue(*vit);

    // append children of this node
    appendAndExpand(child, *it, rootDir, chain);
  }
} // appendAndExpand()


void appendAndExpand(XML::Node &dst, const Path &path, const char *expectedRoot, IncludeChain chain)
{
  const XML::Tree src=readAndParse(path);
  // check if got expected name
  if( src.getRoot().getName()!=expectedRoot )
    throw ConfigFileReader::ExceptionInvalidInclude(SYSTEM_SAVE_LOCATION, path);

  // if name is valid, process this node
  chain.push_back(path);
  const Path rootDir =absoluteDir(path);
  appendAndExpand(dst, src.getRoot(), rootDir, chain);
} // appendAndExpand()


XML::Tree readAndExpand(const Path &path)
{
  try
  {
    XML::Tree tree( (XML::Node(ConfigConsts::configRootNodeName)) );    // default root name
    appendAndExpand( tree.getRoot(), path, tree.getRoot().getName().c_str(), (IncludeChain()) );
    return tree;
  }
  catch(const ConfigFileReader::ExceptionInvalidInclude&)
  {
    throw;
  }
  catch(const ConfigFileReader::ExceptionInclusionLoop&)
  {
    throw;
  }
  catch(const ExceptionFileAccessError &)
  {
    throw;
  }
  catch(const std::exception &ex)
  {
    throw Exception(SYSTEM_SAVE_LOCATION, ex.what());
  }
} // readAndExpand()

} // unnamed namespace


ConfigFileReader::ConfigFileReader(const boost::filesystem::path &path):
  root_( readAndExpand(path) )
{
}

} // namespace ConfigIO
