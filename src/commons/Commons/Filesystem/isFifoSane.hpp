/*
 * isFifoSane.hpp
 *
 */
#ifndef INCLUDE_COMMONS_FIFOSYSTEM_ISFIFOSANE_HPP_FIFO
#define INCLUDE_COMMONS_FIFOSYSTEM_ISFIFOSANE_HPP_FIFO

/* public header */

#include "Base/Filesystem/BoostFS.hpp"
#include "Commons/Filesystem/ExceptionFilesystemIO.hpp"

namespace Commons
{
namespace Filesystem
{

/** \brief check if path does not contain any suspicious elements.
 *  \param p path to fifo.
 *  \return false if path is suspicious, true otherwise.
 *
 *  call checks if non of path elements contains symbolic links.
 */
bool isFifoSane(const Base::Filesystem::Path &p);

} // namespace Filesystem
} // namespace Commons

#endif
