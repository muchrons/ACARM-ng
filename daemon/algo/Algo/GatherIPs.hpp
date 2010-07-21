/*
 * GatherIPs.hpp
 *
 */
#ifndef INCLUDE_ALGO_GATHERIPS_HPP_FILE
#define INCLUDE_ALGO_GATHERIPS_HPP_FILE

/* public header */

#include <map>

#include "Commons/SharedPtrNotNULL.hpp"
#include "Persistency/GraphNode.hpp"
#include "Persistency/Host.hpp"
#include "Algo/forEachUniqueLeaf.hpp"

namespace Algo
{

/** \brief function object collecting all IPs along with their count.
 */
struct GatherIPs
{
public:
  /** \brief counter object.
   */
  class Counter
  {
  public:
    /** \brief start counting with zero.
     */
    Counter(void):
      c_(0)
    {
    }
    /** \brief gets counter's current value.
     *  \return counter value.
     */
    size_t get(void) const
    {
      return c_;
    }
    /** \brief increment counter.
     *  \return incremented counter's value.
     */
    size_t operator++(void)
    {
      return ++c_;
    }

  private:
    size_t c_;
  }; // class Counter

  /** \brief set of hosts. */
  typedef std::map<Persistency::Host::IP, Counter> IPSet;
  /** \brief shared poitner to set to avoid copying. */
  typedef Commons::SharedPtrNotNULL<IPSet>         IPSetPtr;

  /** \brief creates object's instance.
   *  \param node node to take hosts from;
   */
  explicit GatherIPs(Persistency::GraphNodePtrNN node);

  /** \brief adds unique source/target IPs to internall collection.
   *  \param node node (i.e. leaf) to be processed.
   */
  void operator()(Persistency::GraphNodePtrNN node);

  /** \brief gets set of source IPs.
   *  \return hosts' set.
   */
  const IPSet &getSourceIPs(void) const
  {
    return *source_;
  }
  /** \brief gets set of target IPs.
   *  \return hosts' set.
   */
  const IPSet &getTargetIPs(void) const
  {
    return *target_;
  }

private:
  void addIPs(IPSetPtr out, const Persistency::Alert::ReportedHosts &in);

  IPSetPtr source_;
  IPSetPtr target_;
}; // struct GatherIPs

} // namespace Algo

#endif
