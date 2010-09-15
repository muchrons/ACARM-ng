/*
 * ToXML.cpp
 *
 */
#include <algorithm>
#include <cassert>

#include "Commons/Convert.hpp"
#include "Logger/Logger.hpp"
#include "RFCIO/ToXML.hpp"
#include "RFCIO/TimeConverter.hpp"

using namespace Persistency;

namespace RFCIO
{

namespace
{
template<typename T>
std::string toStr(const T &t)
{
  return Commons::Convert::to<std::string>(t);
} // toStr()
} // unnamed namespace


ToXML::ToXML(xmlpp::Element &parent):
  log_("rfcio.toxml"),
  parent_(parent)
{
}

xmlpp::Element &ToXML::addAlert(const Persistency::GraphNode &leaf)
{
  LOGMSG_DEBUG_S(log_)<<"adding alert with ID="<<leaf.getMetaAlert().getID().get();
  const Alert &a=leaf.getAlert();
  ToXML alert( addChild(parent_, "Alert") );
  alert.addParameter("messageid", toStr( leaf.getMetaAlert().getID().get() ).c_str() );
  assert( a.getSourceAnalyzers().begin()!=a.getSourceAnalyzers().end() );
  assert( a.getSourceAnalyzers().begin()->get()!=NULL );
  alert.addAnalyzer( *a.getSourceAnalyzers().begin()->get() );
  // TODO
  return alert.getParent();
}

xmlpp::Element &ToXML::addAnalyzer(const Persistency::Analyzer &a)
{
  LOGMSG_DEBUG_S(log_)<<"adding analyzer with ID="<<a.getID().get();
  ToXML analyzer( addChild(parent_, "Analyzer") );
  analyzer.addParameter("analyzerid", toStr( a.getID().get() ).c_str() );
  analyzer.addParameter("ostype",     a.getOperatingSystem().get() );
  analyzer.addParameter("version",    a.getVersion().get() );
  {
    ToXML node( addChild( analyzer.getParent(), "Node" ) );
    node.addParameter("category", "host");
    node.addString("name", a.getName() );
    if( a.getIP()!=NULL )
    {
      ToXML tmp( node.getParent() );
      tmp.addAddress( *a.getIP() );
    } // if(ip!=NULL)
  } // node
  return analyzer.getParent();
}

xmlpp::Element &ToXML::addCreateTime(const Persistency::Timestamp &t)
{
  return addTimestamp("CreateTime", t);
}

xmlpp::Element &ToXML::addDetectTime(const Persistency::Timestamp &t)
{
  return addTimestamp("DetectTime", t);
}

xmlpp::Element &ToXML::addAddress(const IP &ip)
{
  ToXML address( addChild( getParent(), "Address" ) );
  // determine IP version
  const char *ipType=NULL;
  if( ip.is_v4() )
    ipType="ipv4-addr";
  else
    if( ip.is_v6() )
      ipType="ipv6-addr";
  assert(ipType!=NULL && "unknown address type - update the code!");
  // add proper elements
  address.addParameter("category", ipType);
  address.addString("address", ip.to_string().c_str() );
  // return reference
  return address.getParent();
}

xmlpp::Element &ToXML::addString(const char *name, const char *str)
{
  assert(name!=NULL);
  if(str==NULL)             // NULL string is not possible here
    throw ExceptionUnexpectedNULL(SYSTEM_SAVE_LOCATION, name);
  return addStringImpl(name, str);
}

xmlpp::Element &ToXML::addStringImpl(const char *name, const char *str)
{
  assert(name!=NULL);
  xmlpp::Element &e=addChild(parent_, name);
  if(str!=NULL)
    e.set_child_text(str);
  return e;
}

xmlpp::Element &ToXML::addChild(xmlpp::Element &parent, const char *name)
{
  assert(name!=NULL);
  xmlpp::Element *e=parent.add_child(name);
  if(e==NULL)
  {
    LOGMSG_ERROR_S(log_)<<"addition of child '"<<name<<"' to parent '"<<parent_.get_name()<<"' failed";
    throw ExceptionUnexpectedNULL(SYSTEM_SAVE_LOCATION, "e");
  }
  e->set_namespace( parent.get_namespace_prefix() );
  return *e;
}

void ToXML::addParameter(const char *name, const char *value)
{
  assert(name !=NULL);
  if(value==NULL)
  {
    LOGMSG_DEBUG_S(log_)<<"paramter '"<<name<<"' has NULL value - not adding";
    return;
  }
  LOGMSG_DEBUG_S(log_)<<"setting paramter '"<<name<<"' to "<<value;
  parent_.set_attribute(name, value);
}

xmlpp::Element &ToXML::addTimestamp(const char *name, const Persistency::Timestamp &t)
{
  assert(name!=NULL);
  const TimeConverter  tc;
  xmlpp::Element      &e=addString( name, tc.toString(t).c_str() );
  ToXML                tmp(e);
  tmp.addParameter( "ntpstamp", tc.toNtpStamp(t).c_str() );
  return e;
}

} // namespace RFCIO
