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

/** \brief local cache to store timeouted elements.
 *
 */
class TimeoutedSet
{

private:

  typedef std::vector<std::string> Timeouted;

public:
  /** \brief add timeoted key to the collection.
   *  \param key key to be added.
   */
  void add(std::string &key);
  /** \brief remove timeouted element from collection and from persistency.
   *  \param bf    facade for removing timeouting elements from persistency.
   *  \param owner owner's name.
   */
  void prune(BackendFacade *bf, Persistency::IO::DynamicConfig::Owner &owner);
  /** \brief checks if given key has been alrady timeouted or not.
   *  \param key key from which ...
   *  \return true if key has been already timeouted, false otherwise.
   */
  bool isTimeouted(std::string &key) const;

private:

  Timeouted timeouted_;

}; // class TimeoutedSet
} // namespace Filter
} // namespace NewEvent
#endif
