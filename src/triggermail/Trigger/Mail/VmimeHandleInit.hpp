/*
 * VmimeHandleInit.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_MAIL_VMIMEHANDLEINIT_HPP_FILE
#define INCLUDE_TRIGGER_MAIL_VMIMEHANDLEINIT_HPP_FILE

#include "System/GlobalInit.hpp"
#include "Trigger/Mail/Vmime.hpp"

namespace Trigger
{
namespace Mail
{
namespace detail
{
/** \brief helper, implementaion class.
 */
struct VmimeHandleRegistrator
{
  /** \brief initialize vmime.
   */
  static void init(void)
  {
    vmime::platform::setHandler<vmime::platforms::posix::posixHandler>();
  }
  /** \brief required by API; not needed for vmime.
   */
  static void uninit(void)
  {
  }
}; // struct VmimeHandle
} // namespace detail


/** \brief object for vmime initialization.
 */
typedef System::GlobalInit<detail::VmimeHandleRegistrator> VmimeHandleInit;

} // namespace Mail
} // namespace Trigger

#endif
