/*
 * IDMap.hpp
 *
 */
#ifndef INCLUDE_INPUT_DETAIL_IDMAP_HPP_FILE
#define INCLUDE_INPUT_DETAIL_IDMAP_HPP_FILE

/* public header */

#include <string>
#include <boost/noncopyable.hpp>

#include "Base/Threads/Mutex.hpp"
#include "Commons/SharedPtrNotNULL.hpp"
#include "Persistency/Analyzer.hpp"

// TODO

namespace Input
{
namespace detail
{

/** \brief mapping of input-specific analyzers' IDs to common analyzers' ID scheme.
 */
class IDMap: private boost::noncopyable
{
public:
  /** \brief object to forward mapping saving to persistency.
   */
  struct PersistencyProxy
  {
    virtual ~PersistencyProxy(void);

    virtual void saveMapping(const std::string &inputID, Persistency::Analyzer::ID id) = 0;
  }; // struct PersistencyProxy

  /** \brief constructs instance.
   *  \param nextFreeID next ID that can be used for mapping.
   */
  explicit IDMap(Persistency::Analyzer::ID nextFreeID);

  /** \brief performs mapping from given ID to system-specific one.
   *  \param inputID input-relevant ID to be mapped to our own system's ID.
   *  \param pp      object responsible for saving given mapping to persistent storage.
   *                 note that this object is held by reference and so given object must
   *                 live at least as long as this object's instance.
   *  \return assigned ID.
   *
   *  if given originalID is already mapped, exisitng value is returned. if
   *  new assignment has been created, new, free ID value is returned and
   *  entry is saved to persistent storage.
   */
  Persistency::Analyzer::ID get(PersistencyProxy &pp, const std::string &inputID);

private:
  typedef std::map<std::string, Persistency::Analyzer::ID> MapType;

  Base::Threads::Mutex      mutex_;
  Persistency::Analyzer::ID nextFreeID_;
  MapType                   map_;
}; // class IDMap

} // namespace detail
} // namespace Input

#endif
