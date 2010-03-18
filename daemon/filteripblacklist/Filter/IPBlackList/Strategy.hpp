/*
 * Strategy.hpp
 *
 */
#ifndef INCLUDE_FILTER_IPBLACKLIST_STRATEGY_HPP_FILE
#define INCLUDE_FILTER_IPBLACKLIST_STRATEGY_HPP_FILE

#include <boost/scoped_ptr.hpp>

#include "Persistency/Host.hpp"
#include "Filter/Strategy.hpp"
#include "Filter/IPBlackList/BlackList.hpp"
#include "Filter/IPBlackList/Downloader.hpp"


namespace Filter
{
namespace IPBlackList
{

/** \brief this filter does not need to handle any data.
 */
struct Data
{
}; // struct Data


/** \brief filter detecting multiple attacks from multiple hosts implementation.
 */
class Strategy: public Filter::Strategy<Data>
{
public:
  /** \brief paramters for strategy.
   */
  struct Parameters
  {
    /** \brief create paramters object.
     *  \param refresh  interval between list updates (in seconds).
     *  \param limit    limit number of entries to be read.
     *  \param priDelta increment of priority when host is found.
     */
    Parameters(const unsigned int refresh,
               const unsigned int limit,
               const double       priDelta):
      refresh_(refresh),
      limit_(limit),
      priDelta_(priDelta)
    {
    }

    const unsigned int refresh_;        ///< interval between list refreshing
    const unsigned int limit_;          ///< limit number of entries
    const double       priDelta_;       ///< priority increase
  }; // struct Parameters

  /** \brief create instance.
   *  \param params paramters for dns resolver.
   */
  explicit Strategy(const Parameters &params);

private:
  typedef boost::scoped_ptr<BlackList> BlackListPtr;

  virtual void processImpl(Node               n,
                           NodesTimeoutQueue &ntq,
                           BackendProxy      &bp);

  const Parameters params_;
  time_t           deadline_;
  BlackListPtr     bl_;
  Downloader       dwnl_;
}; // class Strategy

} // namespace IPBlackList
} // namespace Filter

#endif
