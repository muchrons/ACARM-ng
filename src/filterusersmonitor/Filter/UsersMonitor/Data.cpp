/*
 * Data.cpp
 *
 */
#include <algorithm>
#include <cassert>

#include "Filter/UsersMonitor/Data.hpp"

using namespace std;
using namespace Persistency;

namespace Filter
{
namespace UsersMonitor
{

Data::Data(void)
{
}

Data::Data(const Name &name, const Names &skip)
{
  pushAllowed(name, skip);
}

Data::Data(const Persistency::ConstAlertPtrNN &a, const Names &skip)
{
  addFrom( a->getSourceHosts(), skip );
  addFrom( a->getTargetHosts(), skip );
  // elements are kept sorted to make 'common' comparison faster
  sort( names_.begin(), names_.end() );
  // remove duplicates
  names_.erase( unique(names_.begin(), names_.end()), names_.end() );
}

const Data::Names &Data::get(void) const
{
  return names_;
}

Data::Names::const_iterator Data::commonWith(const Data &other) const
{
  Names::const_iterator       it1 =get().begin();
  const Names::const_iterator end1=get().end();
  Names::const_iterator       it2 =other.get().begin();
  const Names::const_iterator end2=other.get().end();

  // knowing that both sets are ordered, we can make fast first-match search
  while(it1!=end1 && it2!=end2)
  {
    // do we have a match?
    if(*it1==*it2)
      return it1;
    // move smaller element forward
    if(*it1<*it2)
      ++it1;
    else
      ++it2;
  }

  //solution not found - return error
  return end1;
}

void Data::swap(Data &other)
{
  names_.swap(other.names_);
}

void Data::addFrom(const Persistency::Alert::Hosts &h, const Names &skip)
{
  for(Alert::Hosts::const_iterator it=h.begin(); it!=h.end(); ++it)
  {
    const Host::Processes &procs=(*it)->getProcesses();
    for(Host::Processes::const_iterator it=procs.begin(); it!=procs.end(); ++it)
    {
      const Process::Username &u=(*it)->getUsername();
      if(u.get()==NULL)                 // no user name?
        continue;
      pushAllowed( u.get(), skip );     // add user to output collection
    } // for(processes)
  } // for(hosts)
}

void Data::pushAllowed(const Name &name, const Names &skip)
{
  // do NOT add elements that are on the skip-list
  if( find(skip.begin(), skip.end(), name)!=skip.end() )
    return;
  // ok - element can be added
  names_.push_back(name);
}

} // namespace UsersMonitor
} // namespace Filter
