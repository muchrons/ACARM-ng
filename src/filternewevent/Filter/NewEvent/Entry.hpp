/*
 * Entry.hpp
 *
 */
#ifndef INCLUDE_FILTER_NEWEVENT_ENTRY_HPP_FILE
#define INCLUDE_FILTER_NEWEVENT_ENTRY_HPP_FILE

#include <openssl/sha.h>
#include "Filter/BackendFacade.hpp"
#include "Filter/Exception.hpp"
#include "Filter/NewEvent/TimeoutedSet.hpp"

namespace Filter
{
namespace NewEvent
{

/** \brief
 *
 */
class Entry
{

public:

  typedef std::string Name;
  typedef std::string Hash;
  /** \brief
   *  \param bf
   *  \param ts
   */
  Entry(Name name, BackendFacade *bf, TimeoutedSet *ts);
  /** \brief
   *
   */
  ~Entry();
  const Entry::Name& getName() const;
  /** \brief
   *
   */
  Hash &getHash();
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
