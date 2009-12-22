/*
 * BackendProxy.hpp
 *
 */
#ifndef INCLUDE_FILTER_BACKENDPROXY_HPP_FILE
#define INCLUDE_FILTER_BACKENDPROXY_HPP_FILE

/* public header */

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include "Persistency/GraphNode.hpp"

// TODO: tests
// TODO: comments

namespace Persistency
{
namespace IO
{
// forward declarations to ommit direct interface usage
class Transaction;
class Connection;
} // namespace IO
} // namespace Persistency

namespace Filter
{

class BackendProxy: private boost::noncopyable
{
public:
  typedef Persistency::GraphNode::ChildrenVector ChildrenVector;

  BackendProxy(Persistency::IO::ConnectionPtrNN  conn,
               const std::string                &filterName);
  ~BackendProxy(void);

  void setHostName(Persistency::HostPtrNN host, const std::string &name);
  void updateSeverityDelta(Persistency::MetaAlertPtrNN ma, double delta);
  void updateCertanityDelta(Persistency::MetaAlertPtrNN ma, double delta);
  void addChild(Persistency::GraphNodePtrNN parent,
                Persistency::GraphNodePtrNN child);
  Persistency::GraphNodePtrNN correlate(
            Persistency::MetaAlertPtrNN  ma,
            Persistency::GraphNodePtrNN  child1,
            Persistency::GraphNodePtrNN  child2,
            const ChildrenVector        &otherChildren=ChildrenVector() );


  void commitChanges(void);

private:
  void beginTransaction(void);
  const Persistency::IO::Transaction &getTransaction(void) const;

  typedef boost::scoped_ptr<Persistency::IO::Transaction> TransactionSPtr;

  std::string                      filterName_;
  TransactionSPtr                  transaction_;
  Persistency::IO::ConnectionPtrNN conn_;
}; // class BackendProxy

} // namespace Filter

#endif
