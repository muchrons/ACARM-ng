/*
 * IDAssigner.cpp
 *
 */
#include <cassert>

#include "System/Threads/SafeInitLocking.hpp"
#include "System/OneInstanceAtOnce.hpp"
#include "Base/Threads/Lock.hpp"
#include "Logger/Logger.hpp"
#include "Commons/Convert.hpp"
#include "Persistency/Facades/IDAssigner.hpp"
#include "Persistency/IO/DynamicConfig.hpp"

using Persistency::IO::DynamicConfig;

namespace Persistency
{
namespace Facades
{
namespace detail
{

namespace
{
const char *g_owner       ="Persistency::IDAssigner";
const char *g_keyMetaAlert="next free MetaAlert's ID";
const char *g_keyAnalyzer ="next free Analyzer's ID";
} // unnamed namespace


/** \brief generic implementation for the assigner
 */
template<typename T>
class IDAssignerHelper: private System::OneInstanceAtOnce< IDAssignerHelper<T> >
{
public:
  /** \brief ID type. */
  typedef typename T::ID           IDType;
  /** \brief numeric equivalent of ID type. */
  typedef typename IDType::Numeric IDNumeric;

  /** \brief create object instance and read initial value of the counter.
   *  \param conn conneciton to use.
   *  \param t    transaction to use.
   *  \param key  to to use for assigning values.
   */
  IDAssignerHelper(IO::ConnectionPtrNN conn, IO::Transaction &t, const char *key):
    log_("persistency.facades.idassigner.idassignerhelper"),
    key_(key),
    nextFreeID_(0u)
  {
    IO::DynamicConfigAutoPtr dc=conn->dynamicConfig(g_owner, t);
    assert( dc.get()!=NULL );

    // read last value from data base
    LOGMSG_DEBUG_S(log_)<<"reading last counter's value (key '"<<key<<"')";
    assert(key_!=NULL);
    const DynamicConfig::ValueNULL r=dc->read(key_);
    // if value is set, counter is already started
    if( r.get()!=NULL )
    {
      LOGMSG_DEBUG_S(log_)<<"value already set to: "<<r.get()->get();
      // parse input value and return to caller
      nextFreeID_=Commons::Convert::to<IDNumeric>( r.get()->get() );
    }
    else
      LOGMSG_DEBUG(log_, "value not set yet - starting counting with 0");
    LOGMSG_DEBUG_S(log_)<<"first free ID (for key '"<<key_<<"') is "<<nextFreeID_;
  }

  /** \brief get next ID value to be used
   *  \param conn connection to persistent storage with counter.
   *  \param t    transaction to use.
   *  \return free ID value, that can be used for new T object.
   */
  IDType assignID(IO::ConnectionPtrNN conn, IO::Transaction &t)
  {
    LOGMSG_DEBUG(log_, "assigning next ID value");
    const IDType assignedID=nextFreeID_;        // save assigned value
    ++nextFreeID_;                              // move to next one
    IO::DynamicConfigAutoPtr dc=conn->dynamicConfig(g_owner, t);
    assert( dc.get()!=NULL );
    // save next free ID
    assert(key_!=NULL);
    dc->write(key_, Commons::Convert::to<std::string>(nextFreeID_) );
    LOGMSG_DEBUG_S(log_)<<"assigned value is "<<assignedID.get();
    LOGMSG_DEBUG_S(log_)<<"next free value is "<<nextFreeID_;

    // return assigned value
    return assignedID;
  }

private:
  Logger::Node  log_;
  const char   *key_;
  IDNumeric     nextFreeID_;
}; // class IDAssignerHelper


/** \brief helper structure that agrtegates all the helpers.
 */
class IDAssigner::IDAssignerPimpl
{
public:
  /** \brief create object.
   *  \param conn connection to be used.
   *  \param t    transaction for initialization operations.
   */
  IDAssignerPimpl(IO::ConnectionPtrNN conn, IO::Transaction &t):
    ma_(conn, t, g_keyMetaAlert),
    a_(conn, t, g_keyAnalyzer)
  {
  }

  /** \brief template for getting assigner for a given ID type (T).
   *  \return assigner helepr of a given type.
   *  \note no default implementation can be provided.
   */
  template<typename T>
  IDAssignerHelper<T> &getAssigner(void);

private:
  IDAssignerHelper<MetaAlert> ma_;
  IDAssignerHelper<Analyzer>  a_;
}; // class IDAssigner::IDAssignerPimpl

template<>
IDAssignerHelper<MetaAlert> &IDAssigner::IDAssignerPimpl::getAssigner(void)
{
  return ma_;
}
template<>
IDAssignerHelper<Analyzer> &IDAssigner::IDAssignerPimpl::getAssigner(void)
{
  return a_;
}


IDAssigner::~IDAssigner(void)
{
  // NOTE: this generates code for deallocating PIMPL in proper way.
}

MetaAlert::ID IDAssigner::assignMetaAlertID(IO::ConnectionPtrNN conn, IO::Transaction &t)
{
  return assign<MetaAlert>(conn, t);
}

Analyzer::ID IDAssigner::assignAnalyzerID(IO::ConnectionPtrNN conn, IO::Transaction &t)
{
  return assign<Analyzer>(conn, t);
}

IDAssigner::IDAssigner(void):
  pimpl_(NULL)
{
  // NOTE: generate valid c-tor
}

template<typename T>
typename T::ID IDAssigner::assign(IO::ConnectionPtrNN conn, IO::Transaction &t)
{
  Base::Threads::Lock lock(mutex_);
  if( pimpl_.get()==NULL )
    pimpl_.reset( new IDAssignerPimpl(conn, t) );
  assert( pimpl_.get()!=NULL );
  return pimpl_->getAssigner<T>().assignID(conn, t);
}

} // namespace detail
} // namespace Facades
} // namespace Persistency
