/*
 * Data.cpp
 *
 */
#include "Filter/UsersMonitor/Data.hpp"

using namespace std;
using namespace Persistency;

namespace Filter
{
namespace UsersMonitor
{

std::auto_ptr<Data> Data::createFrom(const Persistency::ConstAlertPtrNN &a)
{
  auto_ptr<Data> out;
  out=addFrom( out, a->getSourceHosts() );
  out=addFrom( out, a->getTargetHosts() );
  // nothing found?
  if( out.get()==NULL || out->get().size()==0u )
    return auto_ptr<Data>();
  return out;
}

const Data::Names &Data::get(void) const
{
  assert( names_.size()>0u );
  return names_;
}

Data::Names::const_iterator Data::commonWith(const Data &other) const
{
  // TODO
  return names_.end();
}

void Data::swap(Data &other)
{
  names_.swap(other.names_);
}

Data::Data(void)
{
}

std::auto_ptr<Data> Data::addFrom(std::auto_ptr<Data> out, const Persistency::Alert::Hosts &h)
{
  for(Alert::Hosts::const_iterator it=h.begin(); it!=h.end(); ++it)
  {
    const Host::Processes &procs=(*it)->getProcesses();
    for(Host::Processes::const_iterator it=procs.begin(); it!=procs.end(); ++it)
    {
      const Process::Username &u=(*it)->getUsername();
      if(u.get()==NULL)                 // no user name?
        continue;
      if(out.get()==NULL)               // no output object yet?
        out.reset(new Data);
      out->names_.push_back( u.get() ); // add user to output collection
    } // for(processes)
  } // for(hosts)
  // return what has (not) been found
  return out;
}

} // namespace UsersMonitor
} // namespace Filter
