/*
 * Restorer.hpp
 *
 */
#ifndef INCLUDE_PERISITENCY_IO_RESTORER_HPP_FILE
#define INCLUDE_PERISITENCY_IO_RESTORER_HPP_FILE

/* public header */

#include <vector>
#include <boost/noncopyable.hpp>

#include "Persistency/GraphNode.hpp"
#include "Persistency/Timestamp.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Persistency/Exception.hpp"

namespace Persistency
{
namespace IO
{

class Restorer: private boost::noncopyable
{
public:
  struct ExceptionInvalidTimeSpan: public Exception
  {
    ExceptionInvalidTimeSpan(const Location  &where,
                             const Timestamp &from,
                             const Timestamp &to);
  }; // struct ExceptionInvalidTimeSpan

  typedef std::vector<GraphNodePtrNN> NodesVector;

  virtual ~Restorer(void);

  void restoreAllInUse(NodesVector &out);
  void restoreBetween(NodesVector     &out,
                      const Timestamp &from,
                      const Timestamp &to);

protected:
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
