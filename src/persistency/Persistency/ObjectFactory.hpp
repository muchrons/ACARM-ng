/*
 * ObjectFactory.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_OBJECTFACTORY_HPP_FILE
#define INCLUDE_PERSISTENCY_OBJECTFACTORY_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>

#include "Persistency/Transaction.hpp"
#include "Persistency/Alert.hpp"


namespace Persistency
{

/** \brief factory of persistent objects used in program.
 */
class ObjectFactory: private boost::noncopyable
{
public:
  /** \brief auto pointer to transaction.
   */
  typedef std::auto_ptr<Transaction> TransactionAutoPtr;
  /** \brief creates new transaction.
   *  \return transaction object.
   */
  virtual TransactionAutoPtr createTransaction(void) const = 0;

  /** \brief auto pointer to Alert base class.
   */
  typedef std::auto_ptr<Alert> AlertAutoPtr;
  /** \brief creates new alert.
   *  \return aler's interface
   */
  virtual AlertAutoPtr createAlert(/*TODO: parameters to create it from*/) const = 0;

  //
  // TODO: implement creator methods
  //

}; // class ObjectFactory

} // namespace Persistency

#endif
