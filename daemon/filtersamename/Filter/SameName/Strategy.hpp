/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_FILTER_SAMENAME_STRATEGY_HPP_FILE
#define INCLUDE_FILTER_SAMENAME_STRATEGY_HPP_FILE

/* public header */

#include "Persistency/Host.hpp"
#include "Filter/Simple/Strategy.hpp"


namespace Filter
{
namespace SameName
{

/** \brief this filter does not need to handle any data.
 */
struct Data
{
}; // struct Data


// TODO: doxygen to be fixed.
/** \brief filter detecting multiple attacks from multiple hosts implementation.
 */
class Strategy: public Filter::Simple::Strategy<Data>
{
public:
  /** \brief create instance.
   *  \param timeout time observed node should be in queue.
   */
  explicit Strategy(unsigned int timeout);

  /** \brief create ECL for this filter.
   *  \return ECL for filter.
   */
  static Core::Types::Proc::EntryControlList createEntryControlList(void);

private:
  //
  // Simple::Strategy implementation
  //

  virtual NodeEntry makeThisEntry(const Node n) const;
  virtual bool isEntryInteresting(const NodeEntry thisEntry) const;
  virtual Persistency::MetaAlert::Name getMetaAlertName(
                                              const NodeEntry thisEntry,
                                              const NodeEntry otherEntry) const;
  virtual bool canCorrelate(const NodeEntry thisEntry,
                            const NodeEntry otherEntry) const;
}; // class Strategy

} // namespace SameName
} // namespace Filter

#endif
