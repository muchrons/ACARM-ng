/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_FILTER_MANYTOMANY_STRATEGY_HPP_FILE
#define INCLUDE_FILTER_MANYTOMANY_STRATEGY_HPP_FILE

/* public header */

#include "Persistency/Host.hpp"
#include "Filter/Simple/Strategy.hpp"
#include "Filter/ManyToMany/Exception.hpp"


namespace Filter
{
namespace ManyToMany
{

/** \brief this filter does not need to handle any data.
 */
struct Data
{
}; // struct Data


/** \brief filter detecting multiple attacks from multiple hosts implementation.
 */
class Strategy: public Filter::Simple::Strategy<Data>
{
public:
  /** \brief create instance.
   */
  Strategy(void);

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

} // namespace ManyToMany
} // namespace Filter

#endif
