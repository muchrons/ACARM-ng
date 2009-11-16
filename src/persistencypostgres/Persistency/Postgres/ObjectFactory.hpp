/*
 * ObjectFactory.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_POSTGRES__OBJECTFACTORY_HPP_FILE
#define INCLUDE_PERSISTENCY_POSTGRES_OBJECTFACTORY_HPP_FILE

#include "Persistency/ObjectFactory.hpp"


namespace Persistency
{
namespace Postgres
{

/** \brief factory of persistent objects used in program.
 */
class ObjectFactory: public Persistency::ObjectFactory
{
public:
  /** \brief creates new transaction.
   *  \return transaction object.
   */
  virtual TransactionAutoPtr createTransaction(void) const;

  /** \brief creates new alert.
   *  \return aler's interface
   */
  virtual AlertAutoPtr createAlert(/*TODO: parameters to create it from*/) const;

  //
  // TODO: implement creator methods
  //

}; // class ObjectFactory

} // namespace Postgres
} // namespace Persistency

#endif
