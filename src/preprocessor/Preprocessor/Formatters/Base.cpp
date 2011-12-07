/*
 * Base.cpp
 *
 */
#include "Preprocessor/Formatters/Base.hpp"

using namespace std;


namespace Preprocessor
{
namespace Formatters
{

Base::~Base(void)
{
}

std::string Base::exec(void) const
{
  // gather arguments
  Arguments args;
  args.reserve( fmts_.size() );
  for(Formatters::const_iterator it=fmts_.begin(); it!=fmts_.end(); ++it)
  {
    const string &str=(*it)->exec();
    LOGMSG_DEBUG_S(log_)<<"setting argument "<<args.size()<<" to '"<<str<<"'";
    args.push_back(str);
  }
  // call the implementation and return the result
  LOGMSG_DEBUG_S(log_)<<"calling formatter with "<<args.size()<<" arguments";
  const string &str=execImpl(args);
  LOGMSG_DEBUG_S(log_)<<"result is '"<<str<<"'";
  return str;
}

Base::Base(const std::string &name, const Formatters &fmts):
  log_( Logger::NodeName( "preprocessor.formatters.base", Logger::NodeName::removeInvalidChars(name).c_str() ) ),
  fmts_(fmts)
{
  LOGMSG_DEBUG(log_, "creating formatter...");
}

} // namespace Formatters
} // namespace Preprocessor
