/*
 * Operations.hpp
 *
 */
#ifndef INCLUDE_FILTER_ONETOMANY_OPERATIONS_HPP_FILE
#define INCLUDE_FILTER_ONETOMANY_OPERATIONS_HPP_FILE

#include <sstream>

#include "Filter/HostCommon/Strategy.hpp"

namespace Filter
{
namespace OneToMany
{

/** \brief helpers oprations implementation.
 */
struct Operations
{
private:
  typedef Filter::HostCommon::Strategy Types;

public:
  /** \brief computes reoprted host and downs it to one.
   *  \param node node to compute them for.
   *  \return host pointer or NULL if not correlation has been found.
   */
  static Persistency::HostPtr getReportedHost(const Types::Node node)
  {
    // TODO
  }

  static Persistency::MetaAlert::Name getMetaAlertName(const Persistency::HostPtrNN h)
  {
    std::stringstream ss;
    ss << "Multiple attacks from host " << h->getIP();
    if( h->getName().get()!=NULL )
      ss << " (" << h->getName().get() << ")";
    return ss.str();
  }
}; // class Strategy

} // namespace OneToMany
} // namespace Filter

#endif
