/*
 * Factory.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_FACTORY_HPP_FILE
#define INCLUDE_TRIGGER_FACTORY_HPP_FILE

/* public header */

#include <vector>
#include <boost/noncopyable.hpp>

#include "Commons/Factory/AbstractFactorySingleton.hpp"
#include "Core/Types/SignedNodesFifo.hpp"
#include "Core/Types/Proc/Interface.hpp"
#include "Core/Types/Proc/ProcessorsCollection.hpp"


namespace Trigger
{

/** \brief proxy structure for holding created trigger interfaces.
 *
 *  this element is a hack that ensures taht triggers and other elements
 *  (i.e. processors) have separate factories, to avoid name collisions.
 */
struct InterfaceWrapper: private boost::noncopyable
{
  /** \brief short name for pointer held inside. */
  typedef std::auto_ptr<Core::Types::Proc::Interface> InterfaceAutoPtr;

  /** \brief create object.
   *  \param ptr pointer to hold inside.
   */
  explicit InterfaceWrapper(InterfaceAutoPtr ptr):
    ptr_(ptr)
  {
  }

  InterfaceAutoPtr ptr_;    ///< data pointer to hold.
}; // struct InterfaceWrapper


/** \brief abstract factory producing triggers. */
typedef Commons::Factory::AbstractFactorySingleton<InterfaceWrapper> Factory;

/** \brief collection of created triggers. */
typedef Core::Types::Proc::ProcessorsCollection TriggersCollection;

/** \brief creates new input.
 *  \param outputQueue queue to be used as processors' output.
 *  \return newly created input. each call returns new instance.
 */
TriggersCollection create(Core::Types::SignedNodesFifo &outputQueue);

} // namespace Trigger

#endif
