/*
 * AppenderMap.cpp
 *
 */
#include <cassert>

#include "Logger/AppenderMap.hpp"
#include "Logger/Appenders/Console.hpp"
#include "Logger/Appenders/File.hpp"
#include "Logger/Appenders/MultiAppender.hpp"
#include "Logger/Appenders/Null.hpp"

using namespace ConfigIO;

namespace Logger
{

AppenderMap::AppenderMap(const ConfigIO::LoggerAppenders &cfg)
{
  for(LoggerAppenders::const_iterator it=cfg.begin(); it!=cfg.end(); ++it)
    add( it->getName(), configureNew(*it) );
}

Appenders::BasePtr AppenderMap::operator[](const std::string &name) const
{
  AppMap::const_iterator it=apps_.find(name);
  if( it==apps_.end() )
    throw ExceptionNoSuchAppender(SYSTEM_SAVE_LOCATION, name.c_str() );
  assert( it->second.get()!=NULL );
  return it->second;
}

Appenders::BasePtr AppenderMap::configureNew(const ConfigIO::LoggerAppenderConfig &cfg)
{
  //
  // TODO: this should be reworked somehow - it doesn't look nice...
  //
  if( cfg.getType()=="Console" )
  {
    return Appenders::BasePtr(new Appenders::Console);
  }
  else if( cfg.getType()=="File" )
  {
    return Appenders::BasePtr( new Appenders::File( cfg["output"].at(0) ) );
  }
  else if( cfg.getType()=="MultiAppender" )
  {
    typedef ConfigIO::LoggerAppenderConfig::ParameterValues PV; // short for type
    const PV                         &names=cfg["forward"];
    Appenders::MultiAppender::AppVec  apps;
    // add all required appenders to multiappender
    for(PV::const_iterator it=names.begin(); it!=names.end(); ++it)
    {
      Appenders::BasePtr tmp=(*this)[*it];  // get appender by name
      apps.push_back(tmp);                  // add to output collection
    }
    // create new multi appender's instance
    return Appenders::BasePtr( new Appenders::MultiAppender(apps) );
  }
  else if( cfg.getType()=="Null" )
  {
    return Appenders::BasePtr(new Appenders::Null);
  }

  // unknown appender name spotted
  throw ExceptionNoSuchAppenderType(SYSTEM_SAVE_LOCATION,
                                    cfg.getType().c_str() );
}

void AppenderMap::add(const std::string &name, Appenders::BasePtr ptr)
{
  assert( ptr.get()!=NULL );
  apps_.insert( std::make_pair(name, ptr) );
}

} // namespace Logger
