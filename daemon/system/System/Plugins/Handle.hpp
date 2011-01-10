/*
 * Handle.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_PLUGINS_HANDLE_HPP_FILE
#define INCLUDE_SYSTEM_PLUGINS_HANDLE_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>
#include <cassert>

#include "System/SharedPtrNotNULL.hpp"
#include "System/ExceptionPointerIsNULL.hpp"

namespace System
{
namespace Plugins
{

/** \brief helper object owning handle returned by dlopen().
 */
class Handle: private boost::noncopyable
{
public:
  /** \brief create proxy-object.
   *  \param h handle to take (cannot be NULL).
   *  \note object takes ownership of 'h'.
   */
  explicit Handle(void *h);
  /** \brief deallocate handle.
   */
  ~Handle(void);

  /** \brief gets handle.
   *  \return low-level handle.
   */
  void *get(void)
  {
    assert(h_!=NULL);
    return h_;
  }

private:
  void *h_;
}; // class Handle


/** \brief non-NULL pointer to Handle structure. */
typedef SharedPtrNotNULL<Handle> HandlePtrNN;

} // namespace Plugins
} // namespace System

#endif
