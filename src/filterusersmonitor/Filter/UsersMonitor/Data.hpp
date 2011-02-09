/*
 * Data.hpp
 *
 */
#ifndef INCLUDE_FILTER_USERSMONITOR_DATA_HPP_FILE
#define INCLUDE_FILTER_USERSMONITOR_DATA_HPP_FILE

/* public header */

#include <string>
#include <vector>

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

  Data(void);
  explicit Data(const Persistency::ConstAlertPtrNN &a);
  explicit Data(const Name &name);

  const Names &get(void) const;

  Names::const_iterator commonWith(const Data &other) const;

  void swap(Data &other);

private:
  void addFrom(const Persistency::Alert::Hosts &h);

  Names names_;
}; // class Data

} // namespace UsersMonitor
} // namespace Filter

#endif
