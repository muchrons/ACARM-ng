/*
 * Entry.hpp
 *
 */
#ifndef INCLUDE_FILTER_NEWEVENT_ENTRY_HPP_FILE
#define INCLUDE_FILTER_NEWEVENT_ENTRY_HPP_FILE

// TODO: this header is not used here
#include <openssl/sha.h>

#include "Filter/BackendFacade.hpp"
// TODO: according to convention Filter::NewEvent must have its own exception type, derived
//       from Filter::Exception, but cannot throw Filter::Exception directly in own code.
#include "Filter/Exception.hpp"
#include "Filter/NewEvent/TimeoutedSet.hpp"

namespace Filter
{
namespace NewEvent
{

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
  typedef std::string Hash; // consider makeing this LimitedString<> or custom class.

  // TODO: 'name' should be const-ref.
  // TODO: considert taking all arguments by reference and then taking address of an
  //       object. this way you'll avoid a must of checking for NULLs in c-tor and can
  //       stay with asserts(abc!=NULL) in the implemenation.
  /** \brief create instance.
   *  \param name name of processes (meta-)alert.
   *  \param bf   facade for saving elements in the persistency.
   *  \param ts   cache for storing timeouted elements.
   */
  Entry(Name name, Filter::BackendFacade *bf, TimeoutedSet *ts);
  /** \brief delete instance and add hash of timeouted element to the cache.
   */
  ~Entry();

  // TODO: this method is not needed - getHash() is enough.
  /** \brief  get entry name.
   *  \return entry name.
   */
  const Entry::Name& getName() const;
  // TODO: method and its returned value must be const.
  /** \brief return hash of entry name.
   *  \return (SHA1) hash of entry name.
   */
  Hash &getHash();

private:
  // TODO: why not keeping these as a separate values?
  // TODO: std::pair>? requirer 'utility' header.
  typedef std::pair<Name, Hash> Element;

  // computes (SHA1) hash of a given string
  // TODO: this method should be const
  std::string computeHash(const std::string &in);

  Persistency::IO::DynamicConfig::Owner  owner_;
  Persistency::IO::DynamicConfigAutoPtr  dc_;       // TODO: auto_ptr<> is invalid here - Entry is copyied to the collection
                                                    //       and generally expected to behave like a value. make this SharedPtrNotNULL or simillar.
  Element                                element_;
  TimeoutedSet                          *ts_;
  // TODO: invali ddescription
}; // class ProcessedSet

} // namespace Filter
} // namespace NewEvent

#endif
