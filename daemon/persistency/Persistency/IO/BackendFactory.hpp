/*
 * BackendFactory.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_BACKENDFACTORY_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_BACKENDFACTORY_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>

#include "Commons/Factory/AbstractFactorySingleton.hpp"
#include "Persistency/IO/Connection.hpp"

// TODO: test this code

namespace Persistency
{
namespace IO
{

/** \brief wrapper that builds connection's backend of a configured type.
 */
class BackendFactory: private boost::noncopyable
{
public:
  /** \brief backend's singleton type (needed for RegistrationHelper object). */
  typedef Commons::Factory::AbstractFactorySingleton<Connection> Singleton;
  /** \brief connection pointer returned from create() call. */
  typedef Singleton::FactoryPtr                                  FactoryPtr;

  /** \brief creates new connection.
   *  \return newly created connection. each call returns new instance.
   */
  static FactoryPtr create(void);

private:
  BackendFactory(void); // no instances allowed
}; // class BackednFactory

} // namespace IO
} // namespace Persistency

#endif
