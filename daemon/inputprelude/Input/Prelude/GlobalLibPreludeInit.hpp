/*
 * GlobalLibPreludeInit.hpp
 *
 */
#ifndef INCLUDE_INPUT_PRELUDE_GLOBALLIBPRELUDEINIT_HPP_FILE
#define INCLUDE_INPUT_PRELUDE_GLOBALLIBPRELUDEINIT_HPP_FILE

namespace Input
{
namespace Prelude
{

/** \brief helper object for safe initialization of lib-prelude, that cannot
 *         be re-initialized at runtime.
 *
 *  class does automatic prelude initialization uppon first usage, and ensures
 *  proper deinitialization. it ensures that no more than one init will be
 *  done, since prelude does not handle it correctly.
 *
 *  \note class is thread safe.
 */
class GlobalLibPreludeInit
{
public:
  /** \brief initialize prelude, if not already initialized.
   */
  GlobalLibPreludeInit(void);
  /** \brief uninitialize prelude, if needed.
   */
  ~GlobalLibPreludeInit(void);
}; // class GlobalLibPreludeInit

} // namespace Prelude
} // namespace Input

#endif
