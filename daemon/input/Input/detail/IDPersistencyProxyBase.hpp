/*
 * IDPersistencyProxyBase.hpp
 *
 */
#ifndef INCLUDE_INPUT_IDPERSISTENCYPROXYBASE_HPP_FILE
#define INCLUDE_INPUT_IDPERSISTENCYPROXYBASE_HPP_FILE

/* public header */

#include <string>
#include <boost/noncopyable.hpp>

#include "Logger/Node.hpp"
#include "Persistency/Analyzer.hpp"
#include "Persistency/IO/Connection.hpp"
#include "Persistency/IO/Transaction.hpp"

namespace Input
{
namespace detail
{

/** \brief object responsible for I/O operations related to mappings.
 */
class IDPersistencyProxyBase: private boost::noncopyable
{
protected:
  /** \brief create object that operates on default.
   *  \param owner owner of given paramters.
   *  \param conn connection to persistency, to be used.
   *  \param t    transaction to operate on.
   */
  IDPersistencyProxyBase(const std::string                &owner,
                         Persistency::IO::ConnectionPtrNN  conn,
                         Persistency::IO::Transaction     &t):
    log_("input.detail.idpersistencyproxy"),
    conn_(conn),
    t_(t),
    dynConfPtr_( conn->dynamicConfig("Input::"+owner, t_) ),
    dynConf_( mkRef( dynConfPtr_.get() ) )
  {
  }

protected:
  Logger::Node log_;    ///< logger node to use for logging

private:
  template<typename T>
  T &mkRef(T *tmp)
  {
    assert( tmp!=NULL && "framework returned NULL" );
    return *tmp;
  }

  Persistency::IO::ConnectionPtrNN       conn_;
  Persistency::IO::Transaction          &t_;
  Persistency::IO::DynamicConfigAutoPtr  dynConfPtr_;

protected:
  Persistency::IO::DynamicConfig &dynConf_; ///< configuration itself
}; // class IDPersistencyProxyBase

} // namespace detail
} // namespace Input

#endif
