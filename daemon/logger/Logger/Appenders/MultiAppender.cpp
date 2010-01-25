/*
 * MultiAppender.cpp
 *
 */

#include "Logger/Appenders/MultiAppender.hpp"

using namespace std;


namespace Logger
{
namespace Appenders
{

MultiAppender::MultiAppender(const AppVec &apps):
  apps_(apps)
{
  // ensure that all vector elements are not NULL
  for(AppVec::const_iterator it=apps_.begin(); it!=apps_.end(); ++it)
    if( it->get()==NULL )
      throw ExceptionNullParameter(SYSTEM_SAVE_LOCATION, "it->get()");
}

void MultiAppender::appendImpl(const std::string &str)
{
  // output to all registered appenders.
  for(AppVec::const_iterator it=apps_.begin(); it!=apps_.end(); ++it)
    (*it)->append(str);
}

const char *MultiAppender::getTypeNameImpl(void) const
{
  return MultiAppender::getThisTypeName();
}

} // namespace Appenders
} // namespace Logger
