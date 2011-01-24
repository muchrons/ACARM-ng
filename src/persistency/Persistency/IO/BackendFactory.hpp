/*
 * BackendFactory.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_BACKENDFACTORY_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_BACKENDFACTORY_HPP_FILE

/* public header */

#include "Commons/Factory/AbstractFactorySingleton.hpp"
#include "Persistency/IO/Connection.hpp"
#include "Persistency/IO/Exception.hpp"


namespace Persistency
{
namespace IO
{

/** \brief abstract factory producing different persistency backends on demand.
 *  \note this is usefull for creating new backends.
 */
typedef Commons::Factory::AbstractFactorySingleton<Connection> BackendFactory;

/** \brief creates new connection.
 *  \return newly created connection. each call returns new instance.
 */
BackendFactory::FactoryPtr create(void);

} // namespace IO
} // namespace Persistency

#endif
