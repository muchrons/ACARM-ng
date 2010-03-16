/*
 * Interface.hpp
 *
 */
#ifndef INCLUDE_INPUT_INTERFACE_HPP_FILE
#define INCLUDE_INPUT_INTERFACE_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>

#include "Core/Types/AlertsFifo.hpp"
#include "Input/Reader.hpp"

namespace Input
{

/** \brief generic input interface class.
 */
class Interface: private boost::noncopyable
{
public:
  /** \brief create thread running reader in background.
   *  \param reader reader to run in background.
   *  \param output queue to output data to.
   */
  Interface(ReaderPtrNN reader, Core::Types::AlertsFifo &output);

private:
  boost::thread thread_;
}; // class Interface

} // namespace Input

#endif
