/*
 * Base.cpp
 *
 */
#include "Filter/Python/Base.hpp"

namespace Filter
{
namespace Python
{

Base::Base(void):
  log_("filter.python.base")
{
}

Base::~Base(void)
{
}

/*
void Base::filter(const Simple::Strategy::ConstNode &n)
{
  const PythonAPI::Persistency::MetaAlert ma(n);
  LOGMSG_DEBUG_S(log_)<<"node "<<n->getMetaAlert()->getID().get()<<" wrapped as string facade";
  filterImpl(ma);
}
*/

} // namespace Python
} // namespace Filter
