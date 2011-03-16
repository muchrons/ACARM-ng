/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_FILTER_SIMILARITY_STRATEGY_HPP_FILE
#define INCLUDE_FILTER_SIMILARITY_STRATEGY_HPP_FILE

/* public header */

#include "Persistency/Alert.hpp"
#include "Filter/Simple/Strategy.hpp"
#include "Filter/Similarity/ExceptionInvalidParameter.hpp"


namespace Filter
{
namespace Similarity
{

/** \brief keep nodes that comparison should take place for.
 */
struct Data
{
  explicit Data(Persistency::ConstAlertPtrNN first):
    first_(first)
  {
  }

  Data(Persistency::ConstAlertPtrNN first, Persistency::ConstAlertPtrNN second):
    first_(first),
    second_( second.shared_ptr() )
  {
  }

  Persistency::ConstAlertPtrNN first_;
  Persistency::ConstAlertPtr   second_;
}; // struct Data


// TODO: doxygen to be fixed.
/** \brief filter detecting multiple attacks from multiple hosts implementation.
 */
class Strategy: public Filter::Simple::Strategy<Data>
{
public:
  struct Params
  {
    Params(const unsigned int timeout, const double similarity);

    unsigned int timeout(void) const
    {
      return timeout_;
    }

    double similarity(void) const
    {
      return similarity_;
    }

  private:
    unsigned int timeout_;
    double       similarity_;
  }; // struct Params

  /** \brief create instance.
   *  \param name name for strategy.
   *  \param timeout time observed node should be in queue.
   */
  Strategy(const std::string &name, Params p);

  /** \brief create ECL for this filter.
   *  \return ECL for filter.
   */
  static Core::Types::Proc::EntryControlList createEntryControlList(void);

private:
  //
  // Simple::Strategy implementation
  //

  virtual Data makeThisEntryUserData(const Node n) const;
  virtual bool isEntryInteresting(const NodeEntry thisEntry) const;
  virtual Persistency::MetaAlert::Name getMetaAlertName(
                                              const NodeEntry thisEntry,
                                              const NodeEntry otherEntry) const;
  virtual bool canCorrelate(const NodeEntry thisEntry,
                            const NodeEntry otherEntry) const;
  virtual Data makeUserDataForNewNode(const NodeEntry &thisEntry,
                                      const NodeEntry &otherEntry,
                                      const Node       newNode) const;
  virtual void postProcessNode(Node &n, Filter::BackendFacade &bf) const;

  const Params params_;
}; // class Strategy

} // namespace Similarity
} // namespace Filter

#endif
