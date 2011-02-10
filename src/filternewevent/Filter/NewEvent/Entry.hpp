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

/** \brief
 *
 */
class Entry
{

private:

  typedef std::string Name;
  typedef std::string Hash;
  typedef std::pair<Name, Hash> Element;

public:
  Entry(Name name, BackendFacade *bf);
  ~Entry();

  const Name &getName() const;
  const Hash &getHash() const;

  // computes (SHA1) hash of a given string
  std::string computeHash(const std::string &in);

  BackendFacade *bf_;
  Element        element_;

}; // class ProcessedSet
} // namespace Filter
} // namespace NewEvent
#endif
