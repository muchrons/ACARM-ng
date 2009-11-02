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
