/*
 * Factory.hpp
 *
 */
#ifndef INCLUDE_INPUT_FACTORY_HPP_FILE
#define INCLUDE_INPUT_FACTORY_HPP_FILE

/* public header */

#include <vector>

#include "Commons/Factory/AbstractFactorySingleton.hpp"
#include "Core/Types/AlertsFifo.hpp"
#include "Input/Reader.hpp"
#include "Input/Interface.hpp"


namespace Input
{

/** \brief abstract factory producing readers. */
typedef Commons::Factory::AbstractFactorySingleton<Reader> Factory;

/** \brief collection of inputs. */
typedef std::vector<InterfacePtrNN>                        InputsCollection;

/** \brief creates all inputs configured in config file.
 *  \param output     output queue to write to.
 *  \return collection of inputs.
 */
InputsCollection create(Core::Types::AlertsFifo &output);

} // namespace Input

#endif
