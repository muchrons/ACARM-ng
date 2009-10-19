/*
 * nonint_ioctl.hpp
 *
 * non-interruptable ioctl() - continues if interrupt
 * coused returning error from call
 *
 */
#ifndef INCLUDE_SYSTEM_NONINT_IOCTL_HPP_FILE
#define INCLUDE_SYSTEM_NONINT_IOCTL_HPP_FILE

/* public header */

namespace System
{

/** \brief non-EINTR-interruptable ioctl() wrapper.
 *  \param fd   file descriptor of ioctl() to be called on.
 *  \param req  ioctl() number to be called.
 *  \param data data to be passed for ioctl() call.
 *  \return value returned from ioctl.
 */
int nonint_ioctl(int fd, int req, void *data);

} // namespace System

#endif

