/*
 * ReadersFactory.hpp
 *
 */
#ifndef INCLUDE_INPUT_READERSFACTORY_HPP_FILE
#define INCLUDE_INPUT_READERSFACTORY_HPP_FILE

/* public header */

#include "Commons/Factory/AbstractFactorySingleton.hpp"

#include "Input/Reader.hpp"


namespace Input
{

/** \brief abstract factory producing readers.
 */
typedef Commons::Factory::AbstractFactorySingleton<Reader> ReadersFactory;

/** \brief creates new input.
 *  \return newly created input. each call returns new instance.
 */
ReadersFactory::FactoryPtr create(void);

} // namespace Input

#endif
