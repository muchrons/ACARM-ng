/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_FILTER_ONETOONE_STRATEGY_HPP_FILE
#define INCLUDE_FILTER_ONETOONE_STRATEGY_HPP_FILE

/* public header */

#include "Persistency/Host.hpp"
#include "Filter/Simple/Strategy.hpp"


namespace Filter
{
namespace OneToOne
{

/** \brief this filter does not need to handle any data.
 */
struct Data
{
  Data(void):
    isValid_(false)
  {
  }

  Data(const Persistency::Host::IP &from,
       const Persistency::Host::IP &to):
    isValid_(true),
    from_(from),
    to_(to)
  {
  }

  bool                  isValid_;
  Persistency::Host::IP from_;
  Persistency::Host::IP to_;
}; // struct Data


/** \brief filter detecting multiple attacks from given hosts to another.
 */
class Strategy: public Filter::Simple::Strategy<Data>
{
public:
  /** \brief create instance.
   *  \param timeout time observed node should be in queue.
   */
  explicit Strategy(unsigned int timeout);

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

} // namespace OneToOne
} // namespace Filter

#endif
