/*
 * Data.hpp
 *
 */
#ifndef INCLUDE_FILTER_USERSMONITOR_DATA_HPP_FILE
#define INCLUDE_FILTER_USERSMONITOR_DATA_HPP_FILE

/* public header */

#include <string>
#include <vector>
#include <memory>
#include <cassert>

#include "Persistency/Alert.hpp"

namespace Filter
{
namespace UsersMonitor
{

/** \brief data of the filters' observed/correlated entry.
 *
 *  holds entry or a set of user name entries.
 */
class Data
{
public:
  typedef std::string       Name;
  typedef std::vector<Name> Names;

  static std::auto_ptr<Data> createFrom(const Persistency::ConstAlertPtrNN &a);

  const Names &get(void) const;

  Names::const_iterator commonWith(const Data &other) const;

  void swap(Data &other);

private:
  Data(void);

  static std::auto_ptr<Data> addFrom(std::auto_ptr<Data> out, const Persistency::Alert::Hosts &h);

  Names names_;
}; // class Data

} // namespace UsersMonitor
} // namespace Filter

#endif
