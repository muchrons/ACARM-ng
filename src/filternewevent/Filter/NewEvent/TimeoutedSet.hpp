/*
 * TimeoutedSet.hpp
 *
 */
#ifndef INCLUDE_FILTER_NEWEVENT_TIMEOUTEDSET_HPP_FILE
#define INCLUDE_FILTER_NEWEVENT_TIMEOUTEDSET_HPP_FILE

#include "Filter/BackendFacade.hpp"
#include "Filter/Exception.hpp"

namespace Filter
{
namespace NewEvent
{

class Entry;
/** \brief
 *
 */
class TimeoutedSet
{

private:

  typedef std::vector<std::string> Timeouted;

public:
  /** \brief
   *
   */
  void add(std::string &key);
  /** \brief
   *
   */
  void prune(BackendFacade *bf, Persistency::IO::DynamicConfig::Owner &owner);

private:

  Timeouted timeouted_;

}; // class TimeoutedSet
} // namespace Filter
} // namespace NewEvent
#endif
