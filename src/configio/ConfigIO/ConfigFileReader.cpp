/*
 * ConfigFileReader.cpp
 *
 */
#include <vector>

#include "XML/XMLpp/SaxParser.hpp"
#include "ConfigIO/ConfigFileReader.hpp"
#include "ConfigIO/FileReader.hpp"
#include "ConfigIO/Parser.hpp"


namespace ConfigIO
{

ConfigFileReader::ExceptionInclusionLoop::ExceptionInclusionLoop(const Location                &where,
                                                                 const boost::filesystem::path &reIncluded):
  Exception(where, cc("file inclusion loop detected when (re)including file '", reIncluded, "'") )
{
}

ConfigFileReader::ExceptionInvalidInclude::ExceptionInvalidInclude(const Location                &where,
                                                                   const boost::filesystem::path &included):
  Exception(where, cc("inclusion of invalid include file requested: '", included, "'") )
{
}


namespace
{

typedef boost::filesystem::path Path;
typedef std::vector<Path>       IncludeChain;


Path absoluteDir(const Path &in)
{
  // TODO
  return boost::filesystem::absolute(in).parent_path();
}

XML::Tree readAndParse(const Path &path)
{
  FileReader            reader(path);
  XML::XMLpp::SaxParser sax;
  return sax.parseContent( reader.getString() );
} // readAndParse()


void appendAndExpand(Node &dst, const XML::Tree &src, const Path &rootDir)
{
  // if it is an included node, skip root and proceed with it's children
  if(src.getRoot().getName()=="include")
  {
    const XML::Node::TNodesList &children=src.getRoot().getChildrenList();
    for(XML::Node::TNodesList::const_iterator it=children.begin(); it!=children.end(); ++it)
      appendAndExpand(root, *it);
    return;
  }

  // if this is a non-include, just append everything 'as is', except for "acarm_ng" root node
} // appendAndExpand()


XML::Tree readAndExpand(const Path &path)
{
  try
  {
    const Path dir=absoluteDir(path);

    XML::Tree src=readAndParse(path);
    XML::Tree out( Node( src.getRoot().getName() ) );  // copy the name of the original root
    {
      const XML::Node::TNodesList &children=src.getRoot().getChildrenList();
      for(XML::Node::TNodesList::const_iterator it=children.begin(); it!=children.end(); ++it)
        appendAndExpand(root, *it, dir);
    }

    return out;
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
