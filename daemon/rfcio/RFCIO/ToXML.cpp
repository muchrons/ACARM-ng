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

using namespace std;
using namespace Persistency;

namespace RFCIO
{

namespace
{
template<typename T>
string toStr(const T &t)
{
  return Commons::Convert::to<string>(t);
} // toStr()
template<typename T>
string toStr(const T *t)
{
  assert(t!=NULL);
  return toStr(*t);
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
  alert.addCreateTime( a.getCreationTime() );
  if( a.getReportedSourceHosts().size()>0 )
    alert.addSource( *a.getReportedSourceHosts()[0] );
  if( a.getReportedTargetHosts().size()>0 )
    alert.addTarget( *a.getReportedTargetHosts()[0] );
  alert.addClassification(leaf);
  alert.addAdditionalData(leaf);
  // TODO: add rest of the fields
  return alert.getParent();
}

xmlpp::Element &ToXML::addAnalyzer(const Persistency::Analyzer &a)
{
  LOGMSG_DEBUG_S(log_)<<"adding analyzer with ID="<<a.getID().get();
  ToXML analyzer( addChild(parent_, "Analyzer") );
  analyzer.addParameter("analyzerid", toStr( a.getID().get() ).c_str() );
  analyzer.addParameter("ostype",     a.getOperatingSystem().get() );
  analyzer.addParameter("version",    a.getVersion().get() );
  analyzer.addNode( a.getName().get(), a.getIP() );
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

xmlpp::Element &ToXML::addAssessment(const Persistency::GraphNode &leaf)
{
  ToXML assessment( addChild( getParent(), "Assessment" ) );
  // add impact to XML
  {
    const int     minV =Persistency::SeverityLevel::Min;
    const int     sevEn=leaf.getAlert().getSeverity().getLevel().toInt() - minV;    // ensure range starts with 0
    const int     range=leaf.getAlert().getSeverity().getLevel().size()  - 1;       // range is now max value
    assert(range!=0);
    assert( (1.0*sevEn)/range>=0 );
    assert( (1.0*sevEn)/range<=1 );
    const double  sevSm=sevEn + leaf.getMetaAlert().getSeverityDelta();
    const double  sevFl=sevSm/range;
    const double  sev  =min(1.0, max(0.0, sevFl));  // normalize severity to [0;1] range
    assert(sev>=0);
    assert(sev<=1);
    const char   *str  =NULL;;
    if(sev<0.1)
      str="info";
    else
      if(sev<0.3)
        str="low";
      else
        if(sev<0.7)
          str="medium";
        else
          str="high";
    assert(str!=NULL);
    ToXML impact( assessment.addChild( assessment.getParent(), "Impact" ) );
    impact.addParameter("severity", str);
  }
  // add confidence to XML
  {
    const double tmp =leaf.getAlert().getCertainty().get() + leaf.getMetaAlert().getCertaintyDelta();
    const double prob=min(1.0, max(0.0, tmp));          // normalize confidence to [0;1] range
    ToXML confid( assessment.addString( "Confidence", toStr(prob).c_str() ) );
    confid.addParameter("rating", "numeric");
  }
  // return this sub-tree
  return assessment.getParent();
}

xmlpp::Element &ToXML::addClassification(const Persistency::GraphNode &leaf)
{
  ToXML classif( addChild( getParent(), "Classification" ) );
  classif.addParameter( "text", leaf.getMetaAlert().getName().get() );
  const Persistency::ReferenceURL *ref=leaf.getMetaAlert().getReferenceURL();
  if(ref!=NULL)
    classif.addReference(*ref);
  // return reference to newly-created sub-tree
  return classif.getParent();
}

xmlpp::Element &ToXML::addReference(const Persistency::ReferenceURL &ref)
{
  ToXML refElem( addChild( getParent(), "Reference" ) );
  refElem.addParameter("origin", "unknown");
  refElem.addString("name", ref.getName().get() );
  refElem.addString("url",  ref.getURL().get()  );
  return refElem.getParent();
}

xmlpp::Element &ToXML::addAdditionalData(const Persistency::GraphNode &leaf)
{
  ToXML data( addChild( getParent(), "AdditionalData" ) );
  data.addParameter("type", "string");
  data.addParameter("meaning", "description");
  data.addString("string", leaf.getAlert().getDescription().c_str() );
  return data.getParent();
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

xmlpp::Element &ToXML::addService(const Persistency::Service &s)
{
  ToXML service( addChild( getParent(), "Service" ) );
  service.addString( "name", s.getName().get() );
  service.addString( "port", toStr( s.getPort().get() ).c_str() );
  return service.getParent();
}

xmlpp::Element &ToXML::addProcess(const Persistency::Process &p)
{
  ToXML process( addChild( getParent(), "Process") );
  process.addString( "name", p.getName().get() );
  if( p.getPath().get()!=NULL )
    process.addString( "path", p.getPath().get() );
  if( p.getParameters()!=NULL )
    process.addString( "arg", p.getParameters()->c_str() );
  if( p.getPID()!=NULL )
    process.addString( "pid", toStr( p.getPID() ).c_str() );
  return process.getParent();
}

xmlpp::Element &ToXML::addUser(const Persistency::Process &p)
{
  ToXML user( addChild( getParent(), "User") );
  if( p.getUsername().get()==NULL && p.getUID()==NULL )
    return user.getParent();
  // add more data, only if available
  ToXML userId( addChild( user.getParent(), "UserId") );
  if( p.getUsername().get()!=NULL )
    userId.addString("name", p.getUsername().get() );
  if( p.getUID()!=NULL )
    userId.addString("number", toStr( p.getUID() ).c_str() );
  return user.getParent();
}

xmlpp::Element &ToXML::addSource(const Persistency::Host &s)
{
  return addHost("Source", s);
}

xmlpp::Element &ToXML::addTarget(const Persistency::Host &t)
{
  return addHost("Target", t);
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

xmlpp::Element &ToXML::addNode(const char *name, const IP *ip)
{
  ToXML node( addChild( getParent(), "Node" ) );
  const char *category="unknown";
  // add name if present
  if(name!=NULL)
  {
    category="host";
    node.addString("name", name);
  }
  else
  {
    // if name is not set, IP address must be present.
    if(ip==NULL)
      throw ExceptionUnexpectedNULL(SYSTEM_SAVE_LOCATION, "ip");
  }
  // add IP is present
  if(ip!=NULL)
  {
    category="host";
    node.addAddress(*ip);
  }
  // add proper
  node.addParameter("category", category);
  return node.getParent();;
}

xmlpp::Element &ToXML::addHost(const char *rootName, const Persistency::Host &h)
{
  assert(rootName!=NULL);
  ToXML root( addChild( getParent(), rootName ) );
  // add node information
  root.addNode( h.getName().get(), &h.getIP() );
  // add first service, if present (note: IDMEF does not allow multiple services in a single report)
  if( h.getReportedServices().size()>0 )
    root.addService( *h.getReportedServices()[0] );
  // add first process, if present (note: IDMEF does not allow multiple processes in a single report)
  if( h.getReportedProcesses().size()>0 )
  {
    root.addUser( *h.getReportedProcesses()[0] );
    root.addProcess( *h.getReportedProcesses()[0] );
  }
  // done
  return root.getParent();
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
