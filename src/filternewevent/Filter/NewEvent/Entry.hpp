/*
 * Entry.hpp
 *
 */
#ifndef INCLUDE_FILTER_NEWEVENT_ENTRY_HPP_FILE
#define INCLUDE_FILTER_NEWEVENT_ENTRY_HPP_FILE

#include <openssl/sha.h>
#include "Filter/BackendFacade.hpp"
#include "Filter/Exception.hpp"

namespace Filter
{
namespace NewEvent
{

/** \brief forward declaration of TimeoutedSet class.
 */
class TimeoutedSet;
/** \brief
 *  save elements in the persistency dynamic config,
 *  save timeouted elements in dedicated collection.
 */
class Entry
{

public:

  /** \brief entry name type.*/
  typedef std::string Name;
  /** \brief (SHA1) hash of entry name type.*/
  typedef std::string Hash;
  /** \brief create instance.
   *  \param name name of processes (meta-)alert.
   *  \param bf   facade for saving elements in the persistency.
   *  \param ts   cache for storing timeouted elements.
   */
  Entry(Name name, Filter::BackendFacade *bf, TimeoutedSet *ts);
  /** \brief delete instance and add hash of timeouted element to the cache.
   */
  ~Entry();
  /** \brief  get entry name.
   *  \return entry name.
   */
  const Entry::Name& getName() const;
  /** \brief return hash of entry name.
   *  \return (SHA1) hash of entry name.
   */
  const Entry::Hash &getHash() const;

private:
  typedef std::pair<Name, Hash> Element;
  // computes (SHA1) hash of a given string
  std::string computeHash(const std::string &in);

  Persistency::IO::DynamicConfig::Owner  owner_;
  Persistency::IO::DynamicConfigAutoPtr  dc_;
  Element                                element_;
  TimeoutedSet                          *ts_;

}; // class ProcessedSet
} // namespace Filter
} // namespace NewEvent
#endif
