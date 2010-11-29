/*
 * VmimeHandleInit.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_MAIL_VMIMEHANDLEINIT_HPP_FILE
#define INCLUDE_TRIGGER_MAIL_VMIMEHANDLEINIT_HPP_FILE

#include <vmime/platforms/posix/posixHandler.hpp>

#include "System/GlobalInit.hpp"

namespace Trigger
{
namespace Mail
{
namespace detail
{
struct VmimeHandleRegistrator
{
  static void init(void)
  {
    vmime::platform::setHandler<vmime::platforms::posix::posixHandler>();
  }
  static void uninit(void)
  {
  }
}; // struct VmimeHandle
} // namespace detail


typedef System::GlobalInit<detail::VmimeHandleRegistrator> VmimeHandleInit;

} // namespace Mail
} // namespace Trigger

#endif
