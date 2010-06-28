/*
 * IDMap.hpp
 *
 */
#ifndef INCLUDE_INPUT_IDMAP_HPP_FILE
#define INCLUDE_INPUT_IDMAP_HPP_FILE

/* public header */

#include <string>
#include <boost/noncopyable.hpp>

#include "Base/Threads/Mutex.hpp"
#include "Commons/SharedPtrNotNULL.hpp"
#include "Persistency/Analyzer.hpp"

// TODO

namespace Input
{

/** \brief mapping of input-specific analyzers' IDs to common analyzers' ID scheme.
 */
class IDMap: private boost::noncopyable
{
public:
  // TODO: config should be passed as an parameters
  /** \brief constructs instance.
   */
  IDMap(void);

  /** \brief performs mapping from given ID to system-specific one.
   *  \param originalID external ID to be mapped.
   *  \return assigned ID.
   *
   *  if given originalID is already mapped, exisitng value is returned. if
   *  new assignment has been created, new, free ID value is returned and
   *  entry is saved to persistent storage.
   */
  Persistency::Analyzer::ID operator[](const std::string &originalID);

private:
  typedef std::map<std::string, Persistency::Analyzer::ID> MapType;

  Base::Threads::Mutex      mutex_;
  Persistency::Analyzer::ID nextFreeID_;
  MapType                   map_;
}; // class IDMap


typedef Commons::SharedPtrNotNULL<IDMap> IDMapPtrNN;

} // namespace Input

#endif
