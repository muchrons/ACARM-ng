/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_FILTER_SIMILARITY_STRATEGY_HPP_FILE
#define INCLUDE_FILTER_SIMILARITY_STRATEGY_HPP_FILE

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
  /** \brief create data for leaf.
   *  \param first alert to store.
   */
  explicit Data(Persistency::ConstAlertPtrNN first):
    first_(first)
  {
  }
  /** \brief create data from leaf.
   *  \param first  first alert to create from.
   *  \param second second alert to create from.
   */
  Data(Persistency::ConstAlertPtrNN first, Persistency::ConstAlertPtrNN second):
    first_(first),
    second_( second.shared_ptr() )
  {
  }

  Persistency::ConstAlertPtrNN first_;  ///< first alert
  Persistency::ConstAlertPtr   second_; ///< second alert (can be NULL)
}; // struct Data


/** \brief filter correlating similar alerts together.
 */
class Strategy: public Filter::Simple::Strategy<Data>
{
public:
  /** \brief configuration parameters for strategy.
   */
  struct Params
  {
    /** \brief create from given paramters.
     *  \param timeout    maximum time to hold alert for.
     *  \param similarity similarity threshold.
     */
    Params(const unsigned int timeout, const double similarity);

    /** \brief gets timeout value.
     */
    unsigned int timeout(void) const
    {
      return timeout_;
    }
    /** \brief gets similarity threshold.
     */
    double similarity(void) const
    {
      return similarity_;
    }

  private:
    unsigned int timeout_;
    double       similarity_;
  }; // struct Params

  /** \brief create instance.
   *  \param name   name for strategy.
   *  \param params configuration to use.
   */
  Strategy(const Core::Types::Proc::InstanceName &name, Params params);

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
  virtual void postProcessNode(NodeEntry &entry, const NodeEntry &added, BackendFacade &bf) const;


  const Params params_;
}; // class Strategy

} // namespace Similarity
} // namespace Filter

#endif
