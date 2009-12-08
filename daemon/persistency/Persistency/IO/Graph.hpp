/*
 * Graph.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_GRAPH_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_GRAPH_HPP_FILE

/* public header */

#include <memory>
#include <boost/noncopyable.hpp>

//#include "Persistency/Alert.hpp"
#include "Persistency/MetaAlert.hpp"
#include "Persistency/IO/Transaction.hpp"

namespace Persistency
{
namespace IO
{

/** \brief operations on Persistency::Graph* API
 */
class Graph: private boost::noncopyable
{
public:
  explicit Graph(const Transaction &t);

  virtual ~Graph(void);

  void markAsChild(MetaAlertPtr parent, MetaAlertPtr child);
  // TODO
private:
}; // class Graph


typedef std::auto_ptr<Graph> GraphAutoPtr;

} // namespace IO
} // namespace Persistency

#endif
