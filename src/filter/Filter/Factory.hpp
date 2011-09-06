/*
 * Factory.hpp
 *
 */
#ifndef INCLUDE_FILTER_FACTORY_HPP_FILE
#define INCLUDE_FILTER_FACTORY_HPP_FILE

/* public header */

#include <vector>
#include <boost/noncopyable.hpp>

#include "Commons/Factory/AbstractFactorySingleton.hpp"
#include "Core/Types/SignedNodesFifo.hpp"
#include "Core/Types/Proc/Interface.hpp"
#include "Core/Types/Proc/ProcessorsCollection.hpp"


namespace Filter
{

/** \brief proxy structure for holding created filter interfaces.
 *
 *  this element is a hack that ensures taht filters and other elements
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


/** \brief abstract factory producing filters. */
typedef Commons::Factory::AbstractFactorySingleton<InterfaceWrapper> Factory;

/** \brief collection of created filters. */
typedef Core::Types::Proc::ProcessorsCollection FiltersCollection;

/** \brief creates new input.
 *  \param outputQueue queue to be used as processors' output.
 *  \return newly created input. each call returns new instance.
 */
FiltersCollection create(Core::Types::SignedNodesFifo &outputQueue);

} // namespace Filter

#endif
