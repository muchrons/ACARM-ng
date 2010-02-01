/*
 * Restorer.hpp
 *
 */
#ifndef INCLUDE_PERISITENCY_IO_RESTORER_HPP_FILE
#define INCLUDE_PERISITENCY_IO_RESTORER_HPP_FILE

/* public header */

#include <vector>
#include <boost/noncopyable.hpp>

#include "Persistency/GraphNodePtr.hpp"
#include "Persistency/Timestamp.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Persistency/Exception.hpp"

namespace Persistency
{
namespace IO
{

/** \brief class responsible for reading data from persistency.
 */
class Restorer: private boost::noncopyable
{
public:
  /** \brief exception throw when invalid timestamp has been given.
   */
  struct ExceptionInvalidTimeSpan: public Exception
  {
    /** \brief create exception message.
     *  \param where location where exception has been raised.
     *  \param from  begin time stamp.
     *  \param to    end time stam.
     */
    ExceptionInvalidTimeSpan(const Location  &where,
                             const Timestamp &from,
                             const Timestamp &to);
  }; // struct ExceptionInvalidTimeSpan

  /** \brief collection of graph nodes read from DB. */
  typedef std::vector<GraphNodePtrNN> NodesVector;

  /** \brief deallocate object.
   */
  virtual ~Restorer(void);

  /** \brief read all meta-alerts that are in use.
   *  \param out output colleciton ot append nodes to.
   */
  void restoreAllInUse(NodesVector &out);
  /** \brief read meta-alerts created in a given time-range.
   *  \param out  output colleciton ot append nodes to.
   *  \param from begin time stamp.
   *  \param to   end time stamp.
   */
  void restoreBetween(NodesVector     &out,
                      const Timestamp &from,
                      const Timestamp &to);

protected:
  /** \brief create reader for a given transaction.
   *  \param t transaction to be used for reading.
   */
  explicit Restorer(Transaction &t);

private:
  virtual void restoreAllInUseImpl(Transaction &t, NodesVector &out) = 0;
  virtual void restoreBetweenImpl(Transaction     &t,
                                  NodesVector     &out,
                                  const Timestamp &from,
                                  const Timestamp &to) = 0;

  Transaction &t_;
}; // class Restorer

} // namespace IO
} // namespace Persistency

#endif
