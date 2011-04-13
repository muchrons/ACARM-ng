/*
 * Strategy.cpp
 *
 */
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cassert>

#include "System/ignore.hpp"
#include "Filter/SameName/Strategy.hpp"
#include "Algo/forEachUniqueLeaf.hpp"
#include "Algo/GatherHosts.hpp"

using namespace std;
using namespace Persistency;

namespace
{
const char g_filterPrefix[]="[samename] ";

const char *skipFilterPrefix(const char *in)
{
  assert(in!=NULL);
  assert(g_filterPrefix!=NULL);
  // skip prefix, if present
  if( strstr(in, g_filterPrefix)==in )
  {
    const size_t prefixLen=sizeof(g_filterPrefix)-1;
    assert( strlen(in)>=prefixLen );
    return in+prefixLen;
  }
  // return raw string, if no prefix
  return in;
}

}


namespace Filter
{
namespace SameName
{

Strategy::Strategy(const std::string &name, unsigned int timeout):
  Filter::Simple::Strategy<Data>("samename", name, timeout)
{
}

Core::Types::Proc::EntryControlList Strategy::createEntryControlList(void)
{
  Core::Types::Proc::EntryControlList ecl=Core::Types::Proc::EntryControlList::createDefaultReject();
  ecl.add("*input*");   // TODO: magic value
  return ecl;
}

Data Strategy::makeThisEntryUserData(const Node /*n*/) const
{
  return Data();
}

bool Strategy::isEntryInteresting(const NodeEntry /*thisEntry*/) const
{
  // return true beacause meta-alert name is always not null
  return true;
}

Persistency::MetaAlert::Name Strategy::getMetaAlertName(
                                              const NodeEntry thisEntry,
                                              const NodeEntry otherEntry) const
{
  // thisEntry and otherEntry must containt the same meta-alert name
  assert( canCorrelate(thisEntry, otherEntry) );
  System::ignore(otherEntry);
  stringstream ss;
  ss << g_filterPrefix << thisEntry.node_->getMetaAlert()->getName().get();
  return ss.str();
}

bool Strategy::canCorrelate(const NodeEntry thisEntry,
                            const NodeEntry otherEntry) const
{
  // sanityt check
  assert( isEntryInteresting(thisEntry)  );
  assert( isEntryInteresting(otherEntry) );
  // get start pointers
  const char *str1=thisEntry.node_->getMetaAlert()->getName().get();
  const char *str2=otherEntry.node_->getMetaAlert()->getName().get();
  // skip leading correlation marker, if present
  str1=skipFilterPrefix(str1);
  str2=skipFilterPrefix(str2);
  // ok - both names are the same
  if( strcmp(str1, str2)==0 )
    return true;
  return false;
}

Data Strategy::makeUserDataForNewNode(const NodeEntry &/*thisEntry*/,
                                      const NodeEntry &/*otherEntry*/,
                                      const Node       /*newNode*/) const
{
  return Data();
}

void Strategy::postProcessNode(Node &/*n*/, Filter::BackendFacade &/*bf*/) const
{
  // nothing to be done here
}

void Strategy::postProcessNode(NodeEntry &/*entry*/, const NodeEntry &/*added*/, BackendFacade &/*bf*/) const
{
  // nothing to be done here
}

} // namespace SameName
} // namespace Filter
