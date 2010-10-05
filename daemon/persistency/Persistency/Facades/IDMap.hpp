/*
 * IDMap.hpp
 *
 */
#ifndef INCLUDE_INPUT_DETAIL_IDMAP_HPP_FILE
#define INCLUDE_INPUT_DETAIL_IDMAP_HPP_FILE

#include <string>
#include <boost/noncopyable.hpp>

#include "Base/Threads/Mutex.hpp"
#include "Commons/SharedPtrNotNULL.hpp"
#include "Persistency/Analyzer.hpp"
#include "Input/IDPersistencyProxyCommon.hpp"
#include "Input/IDPersistencyProxySpecific.hpp"

// TODO: add protection so that this class is known not to exist in more than 2 instances at once.

namespace Input
{
namespace detail
{

/** \brief mapping of input-specific analyzers' IDs to common analyzers' ID scheme.
 */
class IDMap: private boost::noncopyable
{
public:
  /** \brief constructs instance.
   *  \param nextFreeID next ID that can be used for mapping.
   */
  explicit IDMap(Persistency::Analyzer::ID nextFreeID);

  /** \brief performs mapping from given ID to system-specific one.
   *  \param inputID input-relevant ID to be mapped to our own system's ID.
   *  \param ppc     object responsible for saving next free id mapping to persistent storage.
   *  \param pps     object responsible for saving input-spcific object mappings.
   *  \return assigned ID.
   *
   *  if given originalID is already mapped, exisitng value is returned. if
   *  new assignment has been created, new, free ID value is returned and
   *  entry is saved to persistent storage.
   */
  Persistency::Analyzer::ID get(IDPersistencyProxyCommon   &ppc,
                                IDPersistencyProxySpecific &pps,
                                const std::string          &inputID);

private:
  typedef std::map<std::string, Persistency::Analyzer::ID> MapType;

  Base::Threads::Mutex      mutex_;
  Persistency::Analyzer::ID nextFreeID_;
  MapType                   map_;
}; // class IDMap

} // namespace detail
} // namespace Input

#endif
