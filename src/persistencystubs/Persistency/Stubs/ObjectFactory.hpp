/*
 * ObjectFactory.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_STUBS_OBJECTFACTORY_HPP_FILE
#define INCLUDE_PERSISTENCY_STUBS_OBJECTFACTORY_HPP_FILE

#include "Persistency/ObjectFactory.hpp"


namespace Persistency
{
namespace Stubs
{

/** \brief factory of persistent objects used in program.
 */
class ObjectFactory: public Persistency::ObjectFactory
{
public:
  /** \brief creates new transaction.
   *  \return transaction object.
   */
  virtual TransactionPtr createTransaction(void) const;

  /** \brief creates new alert.
   *  \return aler's interface
   */
  virtual AlertPtr createAlert(/*TODO: parameters to create it from*/) const;

  //
  // TODO: implement creator methods
  //

}; // class ObjectFactory

} // namespace Stubs
} // namespace Persistency

#endif
