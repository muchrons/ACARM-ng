/*
 * Data.hpp
 *
 */
#ifndef INCLUDE_FILTER_USERSMONITOR_DATA_HPP_FILE
#define INCLUDE_FILTER_USERSMONITOR_DATA_HPP_FILE

#include <string>
#include <vector>

#include "Persistency/Alert.hpp"

namespace Filter
{
namespace UsersMonitor
{

/** \brief data of the filters' observed/correlated entry.
 *
 *  holds entry or a set of user name entries from a given alert.
 */
class Data
{
public:
  /** \brief user name type. */
  typedef std::string       Name;
  /** \brief colection of users' names. */
  typedef std::vector<Name> Names;

  /** \brief create empty collection.
   */
  Data(void);
  /** \brief create collection with one name.
   *  \param name name to assign.
   *  \param skip skip list to apply before assigning names.
   *
   *  this call is mostly usefull when createing meta-alert, corelated by a given name.
   */
  Data(const Name &name, const Names &skip);
  /** \brief create colleciton with users from a given alert.
   *  \param a    alert to get user names from.
   *  \param skip skip list to apply before assigning names.
   */
  Data(const Persistency::ConstAlertPtrNN &a, const Names &skip);

  /** \brief gives read-only access to collection of (non-skipped) user names.
   */
  const Names &get(void) const;

  /** \brief returns user name that is common with other data entry.
   *  \param other element co compare with.
   *  \return iterator to the element of collection from this object (see: get())
   *          when common user name is found, or end iterator if not.
   */
  Names::const_iterator commonWith(const Data &other) const;

  /** \brief performs swap operation between two objects.
   *  \param other element to swap contents with.
   */
  void swap(Data &other);

private:
  void addFrom(const Persistency::Alert::Hosts &h, const Names &skip);

  void pushAllowed(const Name &name, const Names &skip);

  Names names_;
}; // class Data

} // namespace UsersMonitor
} // namespace Filter

#endif
