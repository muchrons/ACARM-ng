/*
 * Factory.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_FACTORY_HPP_FILE
#define INCLUDE_TRIGGER_FACTORY_HPP_FILE

/* public header */

#include <vector>

#include "Commons/Factory/AbstractFactorySingleton.hpp"
#include "Core/Types/Proc/Interface.hpp"
#include "Core/Types/Proc/ProcessorsCollection.hpp"


namespace Trigger
{

/** \brief abstract factory producing filters. */
typedef Commons::Factory::AbstractFactorySingleton<Core::Types::Proc::Interface> Factory;

/** \brief collection of created filters. */
typedef Core::Types::Proc::ProcessorsCollection TriggersCollection;

/** \brief creates new input.
 *  \param outputQueue queue to be used as processors' output.
 *  \return newly created input. each call returns new instance.
 */
TriggersCollection create(Core::Types::NodesFifo &outputQueue);

} // namespace Trigger

#endif
