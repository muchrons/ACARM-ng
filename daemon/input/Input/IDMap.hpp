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

class IDMap: private boost::noncopyable
{
public:
  // TODO: config should be passed as an parameters
  IDMap(void);

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
