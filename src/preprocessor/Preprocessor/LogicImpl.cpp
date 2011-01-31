/*
 * LogicImpl.cpp
 *
 */
#include <cassert>

#include "Logger/Logger.hpp"
#include "Preprocessor/LogicImpl.hpp"

namespace Preprocessor
{

LogicImpl::LogicImpl(const ConfigIO::Preprocessor::Config &cfg):
  log_("preprocessor.logicimpl")
{
  typedef ConfigIO::Preprocessor::Config::Sections CfgSections;
  const CfgSections &secs=cfg.getSections();
  LOGMSG_INFO_S(log_)<<"creating from configuration with "<<secs.size()<<" sections defined";

  ss_.reserve( secs.size() );
  // parse all sections and add them to output vector
  for(CfgSections::const_iterator it=secs.begin(); it!=secs.end(); ++it)
    ss_.push_back( Section(*it) );

  assert( ss_.size()==secs.size() );
  LOGMSG_INFO_S(log_)<<"created logic with "<<ss_.size()<<" sections";
}

bool LogicImpl::checkAccept(const Persistency::Alert &alert) const
{
  LOGMSG_DEBUG_S(log_)<<"new alert to check: "<<alert.getName().get();

  // process all section until some match is done
  int i=0;
  for(SectionSet::const_iterator it=ss_.begin(); it!=ss_.end(); ++it)
  {
    ++i;
    switch( it->process(alert).toInt() )
    {
      case Section::Decision::ACCEPT:
        LOGMSG_DEBUG_S(log_)<<"alert '"<<alert.getName().get()<<"' accepted by rule no. "<<i;
        return true;

      case Section::Decision::REJECT:
        LOGMSG_DEBUG_S(log_)<<"alert '"<<alert.getName().get()<<"' denied by rule no. "<<i;
        return false;

      case Section::Decision::CONTINUE:
        break;                                      // go thorught next iteration

      default:
        assert(!"unknown Decision has been made");  // this means error
        break;                                      // by default do nothing
    } // switch(decision)
  } // for(sections)

  // if no section has been matched, by default accept alert
  LOGMSG_DEBUG_S(log_)<<"alert '"<<alert.getName().get()<<"' accepted (default rule)";
  return true;
}

} // namespace Preprocessor
