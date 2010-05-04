/*
 * LogicImpl.cpp
 *
 */
#include <cassert>

#include "Preprocessor/LogicImpl.hpp"

namespace Preprocessor
{

LogicImpl::LogicImpl(const ConfigIO::Preprocessor::Config &cfg)
{
  typedef ConfigIO::Preprocessor::Config::Sections CfgSections;
  const CfgSections &secs=cfg.getSections();

  ss_.reserve( secs.size() );
  // parse all sections and add them to output vector
  for(CfgSections::const_iterator it=secs.begin(); it!=secs.end(); ++it)
    ss_.push_back( Section(*it) );

  assert( ss_.size()==secs.size() );
}

bool LogicImpl::checkAccept(const Persistency::Alert &alert) const
{
  // process all section until some match is done
  for(SectionSet::const_iterator it=ss_.begin(); it!=ss_.end(); ++it)
  {
    switch( it->process(alert).toInt() )
    {
      case Section::Decision::ACCEPT:   return true;
      case Section::Decision::DENY:     return false;
      case Section::Decision::CONTINUE: break;      // go thorught next iteration
      default:
        assert(!"unknown Decision has been made");  // this means error
        break;                                      // by default do nothing
    } // switch(decision)
  } // for(sections)

  // if no section has been matched, by default accept alert
  return true;
}

} // namespace Preprocessor
