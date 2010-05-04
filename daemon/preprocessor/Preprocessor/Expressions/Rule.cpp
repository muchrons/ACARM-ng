/*
 * Rule.cpp
 *
 */
#include <boost/lexical_cast.hpp>
#include <cassert>

#include "Preprocessor/Checkers/Equals.hpp"
#include "Preprocessor/Checkers/Contains.hpp"
#include "Preprocessor/Expressions/Rule.hpp"

using namespace std;


namespace Preprocessor
{
namespace Expressions
{

namespace
{

template<typename T>
string cast(const T &t)
{
  return boost::lexical_cast<string>(t);
} // cast()

template<typename T>
string cast(const T *t)
{
  if(t==NULL)
    return "<NULL>";
  return cast(*t);
} // cast()

template<>
string cast<char>(const char *t)
{
  if(t==NULL)
    return "<NULL>";
  return string(t);
} // cast()

} // unnamed namespace


Rule::Rule(const Path &path, Mode mode, const Value &value):
  path_(path)
{
  // create proper checker
  switch( mode.toInt() )
  {
    case Mode::EQUALS:
      checker_.reset( new Checkers::Equals(value) );
      break;

    case Mode::CONTAINS:
      checker_.reset( new Checkers::Contains(value) );
      break;

    default:
      assert(!"unknown mode requested");
      checker_.reset( new Checkers::Equals(value) );    // fallback...
      break;
  } // switch(mode)
  assert( checker_.get()!=NULL );
}

bool Rule::compute(const Persistency::Alert &alert) const
{
  assert( checker_.get()!=NULL );
  PathCit it=path_.begin();
  if(*it!="alert")
    throwInvalid(SYSTEM_SAVE_LOCATION, it);
  return check( alert, it+1);
}

template<typename T>
bool Rule::processCollection(PathCit t, const T &col) const
{
  if(*t=="*")
    return matchEach( t+1, col.begin(), col.end() );
  if(*t=="$")
    return matchAny( t+1, col.begin(), col.end() );

  throwInvalid(SYSTEM_SAVE_LOCATION, t);
  return false; // code never reaches here
}

template<typename Tit>
bool Rule::matchEach(PathCit t, const Tit begin, const Tit end) const
{
  // for empty set answer is false
  if(begin==end)
    return false;
  // ensure each element is checked
  for(Tit it=begin; it!=end; ++it)
  {
    assert( it->get()!=NULL );
    if( check(*(*it), t)==false )
      return false;
  }
  // if all elements match, result is match as well
  return true;
}

template<typename Tit>
bool Rule::matchAny(PathCit t, const Tit begin, const Tit end) const
{
  // check if any element does match
  for(Tit it=begin; it!=end; ++it)
  {
    assert( it->get()!=NULL );
    if( check(*(*it), t)==true )
      return true;
  }
  // if no elements match, result is no-match as well
  return false;
}

template<typename T>
bool Rule::check(const T *e, PathCit t) const
{
  // NULL object-pointer means that elements does not exist, thus comparison fails
  if(e==NULL)
    return false;
  // for non-NULL perform proper check
  return check(*e, t);
}

bool Rule::check(const Persistency::Alert &e, const PathCit t) const
{
  throwIfEnd(SYSTEM_SAVE_LOCATION, t);

  // direct elements
  if(*t=="name")
    return check( cast( e.getName().get() ), t+1);
  if(*t=="detected")
    return check( cast( e.getDetectionTime() ), t+1);
  if(*t=="created")
    return check( cast( e.getCreationTime() ), t+1);
  if(*t=="certainty")
    return check( cast( e.getCertainty().get() ), t+1);
  if(*t=="severity")
    return check( cast( e.getSeverity().getName() ), t+1);
  if(*t=="description")
    return check( cast( e.getDescription() ), t+1);
  // collections
  if(*t=="analyzers")
    return check( e.getSourceAnalyzers(), t+1);
  if(*t=="sources")
    return check( e.getReportedSourceHosts(), t+1);
  if(*t=="targets")
    return check( e.getReportedTargetHosts(), t+1);

  throwInvalid(SYSTEM_SAVE_LOCATION, t);
  return false;         // we never reach here
}

bool Rule::check(const Persistency::Alert::SourceAnalyzers &e, PathCit t) const
{
  throwIfEnd(SYSTEM_SAVE_LOCATION, t);
  return processCollection(t, e);
}

bool Rule::check(const Persistency::Analyzer &e, PathCit t) const
{
  throwIfEnd(SYSTEM_SAVE_LOCATION, t);

  if(*t=="name")
    return check( cast( e.getName().get() ), t+1 );
  if(*t=="version")
    return check( cast( e.getVersion().get() ), t+1 );
  if(*t=="os")
    return check( cast( e.getOperatingSystem().get() ), t+1 );
  if(*t=="ip")
    return check( cast( (e.getIP()==NULL)?NULL:e.getIP() ), t+1 );

  throwInvalid(SYSTEM_SAVE_LOCATION, t);
  return false;         // we never reach here
}

bool Rule::check(const Persistency::Alert::ReportedHosts &e, PathCit t) const
{
  throwIfEnd(SYSTEM_SAVE_LOCATION, t);
  return processCollection(t, e);
}

bool Rule::check(const Persistency::ReferenceURL &e, PathCit t) const
{
  throwIfEnd(SYSTEM_SAVE_LOCATION, t);

  if(*t=="name")
    return check( cast( e.getName().get() ), t+1 );
  if(*t=="url")
    return check( cast( e.getURL().get() ), t+1 );

  throwInvalid(SYSTEM_SAVE_LOCATION, t);
  return false;         // we never reach here
}

bool Rule::check(const Persistency::Host &e, PathCit t) const
{
  throwIfEnd(SYSTEM_SAVE_LOCATION, t);

  if(*t=="ip")
    return check( cast( e.getIP() ), t+1 );
  if(*t=="mask")
    return check( cast( (e.getNetmask()==NULL)?NULL:e.getNetmask() ), t+1 );
  if(*t=="os")
    return check( cast( e.getOperatingSystem().get() ), t+1 );
  if(*t=="url")
    return check( e.getReferenceURL(), t+1 );
  if(*t=="name")
    return check( cast( e.getName().get() ), t+1 );
  if(*t=="services")
    return check( e.getReportedServices(), t+1 );
  if(*t=="processes")
    return check( e.getReportedProcesses(), t+1 );

  throwInvalid(SYSTEM_SAVE_LOCATION, t);
  return false;         // we never reach here
}

bool Rule::check(const Persistency::Host::ReportedServices &e, PathCit t) const
{
  throwIfEnd(SYSTEM_SAVE_LOCATION, t);
  return processCollection(t, e);
}

bool Rule::check(const Persistency::Service &e, PathCit t) const
{
  throwIfEnd(SYSTEM_SAVE_LOCATION, t);

  if(*t=="name")
    return check( cast( e.getName().get() ), t+1 );
  if(*t=="port")
    return check( cast( e.getPort() ), t+1 );
  if(*t=="protocol")
    return check( cast( e.getProtocol().get() ), t+1 );
  if(*t=="url")
    return check( e.getReferenceURL(), t+1 );

  throwInvalid(SYSTEM_SAVE_LOCATION, t);
  return false;         // we never reach here
}

bool Rule::check(const Persistency::Host::ReportedProcesses &e, PathCit t) const
{
  throwIfEnd(SYSTEM_SAVE_LOCATION, t);
  return processCollection(t, e);
}

bool Rule::check(const Persistency::Process &e, PathCit t) const
{
  throwIfEnd(SYSTEM_SAVE_LOCATION, t);

  if(*t=="path")
    return check( cast( e.getPath().get() ), t+1 );
  if(*t=="name")
    return check( cast( e.getName().get() ), t+1 );
  if(*t=="md5")
    return check( e.getMD5(), t+1 );
  if(*t=="pid")
    return check( cast( e.getPID() ), t+1 );
  if(*t=="uid")
    return check( cast( e.getUID() ), t+1 );
  if(*t=="username")
    return check( cast( e.getUsername().get() ), t+1 );
  if(*t=="arguments")
    return check( cast( e.getParameters() ), t+1 );
  if(*t=="url")
    return check( e.getReferenceURL(), t+1 );

  throwInvalid(SYSTEM_SAVE_LOCATION, t);
  return false;         // we never reach here
}

bool Rule::check(const Persistency::MD5Sum &e, PathCit t) const
{
  return check( cast( e.get() ), t );
}

bool Rule::check(const std::string &e, PathCit t) const
{
  throwIfNotEnd(SYSTEM_SAVE_LOCATION, t);
  assert( checker_.get()!=NULL );
  return checker_->check(e);
}

void Rule::throwIfEnd(const Exception::Location &where, PathCit t) const
{
  if( t==path_.end() )
    throw ExceptionInvalidPath(where, path_.get(), *t);
}

void Rule::throwIfNotEnd(const Exception::Location &where, PathCit t) const
{
  if( t!=path_.end() )
    throw ExceptionInvalidPath(where, path_.get(), *t);
}

void Rule::throwInvalid(const Exception::Location &where, PathCit t) const
{
  throw ExceptionInvalidPath(where, path_.get(), *t);
}

} // namespace Expressions
} // namespace Preprocessor
