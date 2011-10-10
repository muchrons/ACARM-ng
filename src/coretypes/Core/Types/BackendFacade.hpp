/*
 * BackendFacade.hpp
 *
 */
#ifndef INCLUDE_CORE_TYPES_BACKENDFACADE_HPP_FILE
#define INCLUDE_CORE_TYPES_BACKENDFACADE_HPP_FILE

/* public header */

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include "Logger/Node.hpp"
#include "Persistency/IO/Connection.hpp"
#include "Persistency/IO/DynamicConfig.hpp"
#include "Core/Types/Proc/TypeName.hpp"
#include "Core/Types/Proc/InstanceName.hpp"
#include "Core/Types/Proc/CategoryName.hpp"


namespace Core
{
namespace Types
{
/** \brief persistent storage communication simplification layer.
 *
 *  common parts of all backend proxy implementation are gathered here.
 *
 *  \note this object allows transaction's to take place, but only on persistent
 *        level, i.e. rollbacking transaction does not change user objects.
 */
class BackendFacade: private boost::noncopyable
{
public:
  /** \brief base class for performCustomIO() operation.
   *
   *  note that this interface along with performCustomIO method
   *  implements visitor design pattern.
   */
  struct CustomIOInterface: private boost::noncopyable
  {
    /** \brief destroy polymorphic class.
     */
    virtual ~CustomIOInterface(void);
    /** \brief user defined action to occure.
     *  \param conn connection to use for operation.
     *  \param t    transaction to work in.
     */
    virtual void customAction(Persistency::IO::ConnectionPtrNN conn, Persistency::IO::Transaction &t) = 0;
  }; // struct CustomIOInterface

  /** \brief deallocates object's internal resources.
   */
  virtual ~BackendFacade(void);
  /** \brief commit current transaction.
   *
   *  if not transaction is started, call does nothing. if transaction is opened,
   *  it is commited.
   *
   *  \note calling another call after commitChanges will open new transaction.
   */
  void commitChanges(void);
  /** \brief handle to perform implementation-spcific actions, not included in backend facades implementations.
   *  \param ci interface to perform callback on.
   *
   *  this call can be used in specific implementations, that base (i.e. common) interface of
   *  BackendFacade's derives does not handle, since they are specific per-instance.
   */
  void performCustomIO(CustomIOInterface &ci);
  /** \brief send heartbeat, that arrived from an external source (module).
   *  \param m        module's name.
   *  \param validFor maximum ammount of time to wait for next heartbeat.
   *
   *  \note this facility should be used for incomming heartbeats onyl, since heartbeating
   *        for user's implementation is done with other means.
   */
  void heartbeat(const Persistency::IO::Heartbeats::Module &m, unsigned int validFor);
  /** \brief send heartbeat for a given component itself.
   *  \param validFor maximum ammount of time to wait for next heartbeat.
   *
   *  \note some of the elements' implementation send this by default every time. check
   *        the default behavior before sending these heartbeats on own hand.
   */
  void heartbeat(unsigned int validFor);

protected:
  /** \brief create object's instance.
   *  \param conn      connection object to use.
   *  \param category category of element this object is created for (i.e.: filter/trigger/input).
   *  \param type     type of element this object is created for (i.e.: filter/trigger/input name).
   *  \param name     name of element this object is created for (i.e.: filter/trigger/input name).
   */
  BackendFacade(Persistency::IO::ConnectionPtrNN  conn,
                const Proc::CategoryName         &category,
                const Proc::TypeName             &type,
                const Proc::InstanceName         &name);
  /** \brief ensure transaction is in progress (by running new, if needed).
   *  \note if transaciton is started, call does nothing. otherwise starts
   *        new one.
   */
  void beginTransaction(void);
  /** \brief gets transaction object.
   *  \return ongoing transaction.
   */
  Persistency::IO::Transaction &getTransaction(void);
  /** \brief gets connection object.
   *  \return connection object.
   */
  Persistency::IO::ConnectionPtrNN getConnection(void);
  // TODO: consider renaming this method so that it will not be named like public method in derived class
  /** \brief create persistent configuration access object.
   *  \param owner owner's name of a configuration to work on (NULL means common config).
   *  \return return non-NULL dynamic configuration proxy.
   */
  Persistency::IO::DynamicConfigAutoPtr createDynamicConfig(const Persistency::IO::DynamicConfig::Owner &owner);
  /** \brief gets type of this element.
   *  \return elements type.
   */
  const Proc::TypeName &getType(void) const
  {
    return type_;
  }
  /** \brief gets name of this element.
   *  \return elements name.
   */
  const Proc::InstanceName &getName(void) const
  {
    return name_;
  }

private:
  typedef boost::scoped_ptr<Persistency::IO::Transaction> TransactionScPtr;

  std::string getHeartbeatOwnerName(void) const;
  void heartbeatImpl(const char *mod, unsigned int validFor);

  const Logger::Node                       log_;
  const Proc::CategoryName                 category_;
  const Proc::TypeName                     type_;
  const Proc::InstanceName                 name_;
  const Persistency::IO::Heartbeats::Owner hbOwner_;
  Persistency::IO::ConnectionPtrNN         conn_;
  TransactionScPtr                         transaction_;
}; // class BackendFacade

} // namespace Types
} // namespace Core

#endif
