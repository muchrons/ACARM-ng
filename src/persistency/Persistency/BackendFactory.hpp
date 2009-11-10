/*
 * BackendFactory.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_BACKENDFACTORY_HPP_FILE
#define INCLUDE_PERSISTENCY_BACKENDFACTORY_HPP_FILE

/* public header */

#include "Commons/Factory/AbstractFactorySingleton.hpp"

#include "Persistency/ObjectFactory.hpp"


namespace Persistency
{

/** \brief abstract factory producing different persistency backends on demand.
 */
typedef Commons::Factory::AbstractFactorySingleton<ObjectFactory> BackendFactory;

} // namespace Persistency

#endif
