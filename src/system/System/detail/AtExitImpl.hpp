/*
 * AtExitImpl.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_DETAIL_ATEXITIMPL_HPP_FILE
#define INCLUDE_SYSTEM_DETAIL_ATEXITIMPL_HPP_FILE

#include <stack>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "System/AtExitResourceDeallocator.hpp"
#include "System/AtExit.hpp"


namespace System
{
namespace detail
{

/** \brief implementation class of AtExit.
 *  \note since AtExit is thread-safe, this one is not.
 */
class AtExitImpl: private boost::noncopyable
{
public:
  /** \brief create object.
   */
  AtExitImpl(void);
  /** \brief destroy object and ensure deallocateAll() has been called.
   */
  ~AtExitImpl(void);
  /** \brief deallocates all registered resources.
   *  \note this method should be called exactly once during lifetime
   *        of created object. no registerations can be done after this
   *        method has been called.
   */
  void deallocateAll(void);
  /** \brief registers new deallocator.
   *  \param ptr deallocator to be registered.
   */
  void registerDeallocator(AtExit::TDeallocPtr ptr);

private:
  typedef boost::shared_ptr<AtExitResourceDeallocator> Elem;
  typedef std::stack<Elem>                             Stack;

  Stack deallocators_;
  bool  deallocationDone_;
}; // class AtExitImpl

} // namespace detail
} // namespace System

#endif
