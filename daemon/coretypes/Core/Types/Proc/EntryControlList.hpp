/*
 * EntryControlList.hpp
 *
 */
#ifndef INCLUDE_CORE_TYPES_PROC_ENTRYCONTROLLIST_HPP_FILE
#define INCLUDE_CORE_TYPES_PROC_ENTRYCONTROLLIST_HPP_FILE

/* public header */

#include <string>
#include <vector>

namespace Core
{
namespace Types
{
namespace Proc
{

/** \brief control list to allow limiting entries that can enter given processor.
 *
 *  this class has two modes - it can accept all entries, but the ones listed explicitly
 *  (EntryControlList::createDefaultAccept()) OR it can reject all entries, but the
 *  ones listed explicitly (EntryControlList::createDefaultReject()). regardless of
 *  mode, entries that are to be accepted/rejected have to be (explicitly) added
 *  with add() method.
 */
class EntryControlList
{
public:
  /** \brief type representing filter's to operate on. */
  typedef std::string FilterName;

  /** \brief create object in mode 'accept by default', with rejecting only explicitly
   *         added entries.
   *  \return ECL's class object.
   */
  static EntryControlList createDefaultAccept(void);
  /** \brief create object in mode 'reject by default', with accepting only explicitly
   *         added entries.
   *  \return ECL's class object.
   */
  static EntryControlList createDefaultReject(void);

  /** \brief add new entry to check-list.
   *  \param filterName name of the filter to be added to access list.
   */
  void add(const FilterName &filterName);
  /** \brief test if given entry should be accepted or rejected.
   *  \param filterName filter name to be tested.
   *  \return true if entry is to be accepted, false otherwise.
   */
  bool isAcceptable(const FilterName &filterName) const;

private:
  // this class uses 'named c-tor' design pattern, thus 'real' c-tor
  // is private here.
  explicit EntryControlList(bool acceptByDefault);

  typedef std::vector<FilterName> FilterNamesVector;

  FilterNamesVector filterNames_;
  bool              defaultPolicy_;
}; // class EntryControlList

} // namespace Proc
} // namespace Types
} // namespace Core

#endif
